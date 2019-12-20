#include "FastMagicBitboards.hpp"

FastMagicBitboards::FastMagicBitboards() : _randomGenerator(_randomDevice()), _randomDistribution(0, 0x3ffffffffffffff)
{
	rookAttacksGenerator = std::make_unique<RookAttacksGenerator>();
	bishopAttacksGenerator = std::make_unique<BishopAttacksGenerator>();
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
	assert(shift != 0);

	U64 mask = 0;
	for (int i = BitboardOperations::DistanceToEdge(field, shift); i >= 0; i--)
	{
		mask |= (U64)1 << field;
		field += shift;
	}

	return mask;
}

void FastMagicBitboards::calculateField(int field, std::array<MagicStructure, 64> &pieceMagicStructures, std::unique_ptr<AttacksGeneratorBase> &attacksGenerator)
{
	assert(field >= 0 && field < 64);

	int permutationsCount = 1 << BitOperations::Count(pieceMagicStructures[field].Mask);
	auto permutations = std::make_unique<U64 []>(permutationsCount);
	auto attacks = std::make_unique<U64[]>(permutationsCount);

	for (int p = 0; p < permutationsCount; p++)
	{
		permutations[p] = generatePermutation(p, field, pieceMagicStructures[field].Mask);
		attacks[p] = attacksGenerator->Generate(field, permutations[p]);
	}

	pieceMagicStructures[field].MagicNumber = generateMagicNumber(pieceMagicStructures[field], permutations, attacks);
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

__declspec(noinline)  U64 FastMagicBitboards::generateMagicNumber(MagicStructure &pieceMagicStructures, std::unique_ptr<U64 []> &permutations, std::unique_ptr<U64[]> &attacks)
{
	int attacksCount = 1 << BitOperations::Count(pieceMagicStructures.Mask);
	int shift = BitOperations::Count(pieceMagicStructures.Mask);
	int magicOffset = 64 - shift;

	pieceMagicStructures.MagicAttacks = std::make_unique<Bitboard []>((int)(1 << shift));

	bool success = false;
	while (!success)
	{
		U64 magicNumber = randU64() & randU64() & randU64();
		magicNumber |= (U64)magicOffset << 58;

		memset(pieceMagicStructures.MagicAttacks.get(), 0, attacksCount * sizeof(Bitboard));

		success = true;
		for (int i = 0; i < attacksCount; i++)
		{
			U64 hash = permutations[i] * magicNumber;
			int index = (int)(hash >> magicOffset);

			if (pieceMagicStructures.MagicAttacks[index] != 0 && pieceMagicStructures.MagicAttacks[index] != attacks[i])
			{
				success = false;
				break;
			}

			pieceMagicStructures.MagicAttacks[index] = attacks[i];
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