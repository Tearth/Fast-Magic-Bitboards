#include "FastMagicBitboards.hpp"

FastMagicBitboards::FastMagicBitboards() : _randomGenerator(_randomDevice()), _randomDistribution(0, 0x3ffffffffffffff)
{
	rookAttacksGenerator = new RookAttacksGenerator();
	bishopAttacksGenerator = new BishopAttacksGenerator();

	memset(_rookMagicStructures, 0, sizeof(MagicStructure) * 64);
	memset(_bishopMagicStructures, 0, sizeof(MagicStructure) * 64);
}

FastMagicBitboards::~FastMagicBitboards()
{

}

void FastMagicBitboards::GenerateForRook()
{
	for (int i = 0; i < 64; i++)
	{
		GenerateForRook(i);
	}
}

void FastMagicBitboards::GenerateForRook(int field)
{
	assert(field >= 0 && field < 64);

	_rookMagicStructures[field].Mask = generateRookMask(field);
	calculateField(field, _rookMagicStructures, rookAttacksGenerator);
}

void FastMagicBitboards::GenerateForBishop()
{
	for (int i = 0; i < 64; i++)
	{
		GenerateForBishop(i);
	}
}

void FastMagicBitboards::GenerateForBishop(int field)
{
	assert(field >= 0 && field < 64);

	_bishopMagicStructures[field].Mask = generateBishopMask(field);
	calculateField(field, _bishopMagicStructures, bishopAttacksGenerator);
}

Bitboard FastMagicBitboards::GetRookAttacks(int field, Bitboard occupancy)
{
	assert(field >= 0 && field < 64);

	occupancy &= _rookMagicStructures[field].Mask;
	occupancy *= _rookMagicStructures[field].MagicNumber;
	occupancy >>= _rookMagicStructures[field].MagicNumber >> 58;
	return _rookMagicStructures[field].MagicAttacks[occupancy];
}

Bitboard FastMagicBitboards::GetBishopAttacks(int field, Bitboard occupancy)
{
	assert(field >= 0 && field < 64);

	occupancy &= _bishopMagicStructures[field].Mask;
	occupancy *= _bishopMagicStructures[field].MagicNumber;
	occupancy >>= _rookMagicStructures[field].MagicNumber >> 58;
	return _bishopMagicStructures[field].MagicAttacks[occupancy];
}

U64 FastMagicBitboards::generateRookMask(int field)
{
	assert(field >= 0 && field < 64);

	const U64 firstFile = 0x101010101010101;
	const U64 firstRank = 0xff;
	const U64 bitboardWithoutFirstAndLastFile = 0x7e7e7e7e7e7e7e7e;
	const U64 bitboardWithoutFirstAndLastRank = 0xffffffffffff00;

	U64 mask = 0;
	mask |= (firstFile << BitboardOperations::FieldToFile(field)) & bitboardWithoutFirstAndLastRank;
	mask |= (firstRank << (BitboardOperations::FieldToRank(field) * 8)) & bitboardWithoutFirstAndLastFile;
	mask &= ~((U64)1 << field);

	return mask;
}

U64 FastMagicBitboards::generateBishopMask(int field)
{
	assert(field >= 0 && field < 64);

	U64 mask = 0;
	mask |= generateMaskForDirection(field + 7, 7);
	mask |= generateMaskForDirection(field - 7, -7);
	mask |= generateMaskForDirection(field + 9, 9);
	mask |= generateMaskForDirection(field - 9, -9);

	return mask;
}

U64 FastMagicBitboards::generateMaskForDirection(int field, int shift)
{
	assert(field >= 0 && field < 64);
	assert(shift != 0);

	const U64 bitboardEdges = 0xff818181818181ff;

	U64 mask = 0;
	while (((U64)1 << field & bitboardEdges) == 0)
	{
		mask |= (U64)1 << field;
		field += shift;
	}

	return mask;
}

void FastMagicBitboards::calculateField(int field, MagicStructure *pieceMagicStructures, AttacksGeneratorBase *attacksGenerator)
{
	assert(field >= 0 && field < 64);

	int permutationsCount = 1 << BitOperations::Count(pieceMagicStructures[field].Mask);
	U64 *permutations = new U64[permutationsCount];
	U64 *attacks = new U64[permutationsCount];

	for (int p = 0; p < permutationsCount; p++)
	{
		permutations[p] = generatePermutation(p, field, pieceMagicStructures[field].Mask);
		attacks[p] = attacksGenerator->Generate(field, permutations[p]);
	}

	pieceMagicStructures[field].MagicNumber = generateMagicNumber(&pieceMagicStructures[field], permutations, attacks);
	delete[] permutations;
	delete[] attacks;
}

U64 FastMagicBitboards::generatePermutation(int permutationIndex, int field, U64 mask)
{
	assert(permutationIndex >= 0);
	assert(field >= 0 && field < 64);

	U64 permutation = 0;
	while (mask != 0)
	{
		U64 lsb = BitOperations::GetLSB(mask);
		mask = BitOperations::PopLSB(mask);

		if ((permutationIndex & 1) != 0)
		{
			int lsbIndex = BitOperations::BitScan(lsb);
			permutation |= (U64)1 << lsbIndex;
		}

		permutationIndex >>= 1;
	}

	return permutation;
}

U64 FastMagicBitboards::generateMagicNumber(MagicStructure *pieceMagicStructures, U64 *permutations, Bitboard *attacks)
{
	int attacksCount = 1 << BitOperations::Count(pieceMagicStructures->Mask);
	int shift = BitOperations::Count(pieceMagicStructures->Mask);
	bool success = false;

	if (pieceMagicStructures->MagicAttacks != nullptr)
	{
		delete[] pieceMagicStructures->MagicAttacks;
	}
	pieceMagicStructures->MagicAttacks = new Bitboard[(int)(1 << shift)];

	while (!success)
	{
		U64 magicNumber = randU64() & randU64() & randU64();
		magicNumber |= ((U64)64 - shift) << 58;

		memset(pieceMagicStructures->MagicAttacks, 0, attacksCount * sizeof(Bitboard));

		success = true;
		for (int i = 0; i < attacksCount; i++)
		{
			U64 hash = permutations[i] * magicNumber;
			int index = (int)(hash >> (64 - shift));

			if (pieceMagicStructures->MagicAttacks[index] != 0 && pieceMagicStructures->MagicAttacks[index] != attacks[i])
			{
				success = false;
				break;
			}

			pieceMagicStructures->MagicAttacks[index] = attacks[i];
		}

		if (success)
		{
			return magicNumber;
		}
	}

	return -1;
}

U64 FastMagicBitboards::randU64()
{
	return _randomDistribution(_randomGenerator);
}