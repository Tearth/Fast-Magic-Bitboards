#include "FastMagicBitboards.hpp"

FastMagicBitboards::FastMagicBitboards()
{
}

FastMagicBitboards::~FastMagicBitboards()
{

}

void FastMagicBitboards::GenerateForRook(int field)
{
	_rookMagicStructures[field].Mask = generateRookMask(field);
	_rookMagicStructures[field].Shift = BitOperations::Count(_rookMagicStructures[field].Mask);
	generateAttacks(field, _rookMagicStructures, _rookMagicAttacks, &FastMagicBitboards::generateRookAttacks);

	std::cout << _rookMagicStructures[field].MagicNumber << endl;
}

void FastMagicBitboards::GenerateForBishop(int field)
{
	_bishopMagicStructures[field].Mask = generateBishopMask(field);
	_bishopMagicStructures[field].Shift = BitOperations::Count(_bishopMagicStructures[field].Mask);
	generateAttacks(field, _bishopMagicStructures, _bishopMagicAttacks, &FastMagicBitboards::generateBishopAttacks);

	std::cout << _bishopMagicStructures[field].MagicNumber << endl;
}

U64 FastMagicBitboards::generateRookMask(int field)
{
	const U64 firstFile = 0x101010101010101;
	const U64 firstRank = 0xff;
	const U64 bitboardWithoutFirstAndLastFile = 0x7e7e7e7e7e7e7e7e;
	const U64 bitboardWithoutFirstAndLastRank = 0xffffffffffff00;

	U64 mask = 0;
	mask |= (firstFile << fieldToFile(field)) & bitboardWithoutFirstAndLastRank;
	mask |= (firstRank << (fieldToRank(field) * 8)) & bitboardWithoutFirstAndLastFile;
	mask &= ~((U64)1 << field);

	return mask;
}

U64 FastMagicBitboards::generateBishopMask(int field)
{
	U64 mask = 0;
	mask |= generateMaskForDirection(field + 7, 7);
	mask |= generateMaskForDirection(field - 7, -7);
	mask |= generateMaskForDirection(field + 9, 9);
	mask |= generateMaskForDirection(field - 9, -9);

	return mask;
}

U64 FastMagicBitboards::generateMaskForDirection(int field, int shift)
{
	const U64 bitboardEdges = 0xff818181818181ff;

	U64 mask = 0;
	while (((U64)1 << field & bitboardEdges) == 0)
	{
		mask |= (U64)1 << field;
		field += shift;
	}

	return mask;
}

void FastMagicBitboards::generateAttacks(int field, MagicStructure *pieceMagicStructures, Bitboard **pieceAttacks, U64 (FastMagicBitboards::*attacksGenerator)(int, U64))
{
	int permutationsCount = 1 << BitOperations::Count(pieceMagicStructures[field].Mask);
	U64 *permutations = new U64[permutationsCount];
	U64 *attacks = new U64[permutationsCount];

	for (int p = 0; p < permutationsCount; p++)
	{
		permutations[p] = generatePermutation(p, field, pieceMagicStructures[field].Mask);
		attacks[p] = (this->*attacksGenerator)(field, permutations[p]);
	}

	_rookMagicStructures[field].MagicNumber = generateMagicNumber(&_rookMagicStructures[field], _rookMagicAttacks[field], permutations, attacks);
	delete[] permutations;
	delete[] attacks;
}

U64 FastMagicBitboards::generatePermutation(int permutationIndex, int field, U64 mask)
{
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

U64 FastMagicBitboards::generateRookAttacks(int field, U64 occupancy)
{
	U64 attacks = 0;
	attacks |= generateAttacksForDirection(field, 1, occupancy);
	attacks |= generateAttacksForDirection(field, -1, occupancy);
	attacks |= generateAttacksForDirection(field, 8, occupancy);
	attacks |= generateAttacksForDirection(field, -8, occupancy);

	return attacks;
}

U64 FastMagicBitboards::generateBishopAttacks(int field, U64 occupancy)
{
	U64 attacks = 0;
	attacks |= generateAttacksForDirection(field, 7, occupancy);
	attacks |= generateAttacksForDirection(field, -7, occupancy);
	attacks |= generateAttacksForDirection(field, 9, occupancy);
	attacks |= generateAttacksForDirection(field, -9, occupancy);
	
	return attacks;
}

U64 FastMagicBitboards::generateAttacksForDirection(int field, int shift, U64 occupancy)
{
	U64 attacks = 0;
	for (int i = distanceToEdge(field, shift); i > 0 && (((U64)1 << field) & occupancy) == 0; i--)
	{
		field += shift;
		attacks |= (U64)1 << field;
	}

	return attacks;
}

U64 FastMagicBitboards::generateMagicNumber(MagicStructure *pieceMagicStructures, Bitboard *pieceMagicAttacks, U64 *permutations, Bitboard *attacks)
{
	int attacksCount = 1 << BitOperations::Count(pieceMagicStructures->Mask);
	pieceMagicAttacks = new Bitboard[1 << pieceMagicStructures->Shift];
	bool success = false;

	while (!success)
	{
		U64 magicNumber = randU64();
		memset(pieceMagicAttacks, 0, attacksCount * sizeof(U64));

		success = true;
		for (int i = 0; i < attacksCount; i++)
		{
			U64 hash = permutations[i] * magicNumber;
			int index = (int)(hash >> (64 - pieceMagicStructures->Shift));

			if (pieceMagicAttacks[index] != 0 && pieceMagicAttacks[index] != attacks[i])
			{
				success = false;
				break;
			}

			pieceMagicAttacks[index] = attacks[i];
		}

		if (success)
		{
			return magicNumber;
		}
	}

	return -1;
}

int FastMagicBitboards::distanceToEdge(int field, int shift)
{
	switch (shift)
	{
	case 1:  return 7 - fieldToFile(field);
	case -1: return fieldToFile(field);
	case 8:  return 7 - fieldToRank(field);
	case -8: return fieldToRank(field);
	case 9:  return min(7 - fieldToFile(field), 7 - fieldToRank(field));
	case -7: return min(7 - fieldToFile(field), fieldToRank(field));
	case -9: return min(fieldToFile(field), fieldToRank(field));
	case 7:  return min(fieldToFile(field), 7 - fieldToRank(field));
	}

	assert(1);
	return -1;
}

int FastMagicBitboards::fieldToFile(int field)
{
	return field % 8;
}

int FastMagicBitboards::fieldToRank(int field)
{
	return field / 8;
}

U64 FastMagicBitboards::randU64()
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<U64> distribution(0, 0xFFFFFFFFFFFFFFFF);

	return distribution(generator) & distribution(generator) & distribution(generator);
}