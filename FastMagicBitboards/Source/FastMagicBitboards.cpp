#include "FastMagicBitboards.hpp"

FastMagicBitboards::FastMagicBitboards() : _randomGenerator(_randomDevice()), _randomDistribution(0, 0x3ffffffffffffff)
{
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
	generateAttacks(field, _rookMagicStructures, &FastMagicBitboards::generateRookAttacks);
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
	generateAttacks(field, _bishopMagicStructures, &FastMagicBitboards::generateBishopAttacks);
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
	mask |= (firstFile << fieldToFile(field)) & bitboardWithoutFirstAndLastRank;
	mask |= (firstRank << (fieldToRank(field) * 8)) & bitboardWithoutFirstAndLastFile;
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

void FastMagicBitboards::generateAttacks(int field, MagicStructure *pieceMagicStructures, U64 (FastMagicBitboards::*attacksGenerator)(int, U64))
{
	assert(field >= 0 && field < 64);

	int permutationsCount = 1 << BitOperations::Count(pieceMagicStructures[field].Mask);
	U64 *permutations = new U64[permutationsCount];
	U64 *attacks = new U64[permutationsCount];

	for (int p = 0; p < permutationsCount; p++)
	{
		permutations[p] = generatePermutation(p, field, pieceMagicStructures[field].Mask);
		attacks[p] = (this->*attacksGenerator)(field, permutations[p]);
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

U64 FastMagicBitboards::generateRookAttacks(int field, U64 occupancy)
{
	assert(field >= 0 && field < 64);

	U64 attacks = 0;
	attacks |= generateAttacksForDirection(field, 1, occupancy);
	attacks |= generateAttacksForDirection(field, -1, occupancy);
	attacks |= generateAttacksForDirection(field, 8, occupancy);
	attacks |= generateAttacksForDirection(field, -8, occupancy);

	return attacks;
}

U64 FastMagicBitboards::generateBishopAttacks(int field, U64 occupancy)
{
	assert(field >= 0 && field < 64);

	U64 attacks = 0;
	attacks |= generateAttacksForDirection(field, 7, occupancy);
	attacks |= generateAttacksForDirection(field, -7, occupancy);
	attacks |= generateAttacksForDirection(field, 9, occupancy);
	attacks |= generateAttacksForDirection(field, -9, occupancy);
	
	return attacks;
}

U64 FastMagicBitboards::generateAttacksForDirection(int field, int shift, U64 occupancy)
{
	assert(field >= 0 && field < 64);
	assert(shift != 0);

	U64 attacks = 0;
	for (int i = distanceToEdge(field, shift); i > 0 && (((U64)1 << field) & occupancy) == 0; i--)
	{
		field += shift;
		attacks |= (U64)1 << field;
	}

	return attacks;
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

int FastMagicBitboards::distanceToEdge(int field, int shift)
{
	assert(field >= 0 && field < 64);
	assert(shift != 0);

	int distance;
	switch (shift)
	{
	case 1:  distance = 7 - fieldToFile(field); break;
	case -1: distance = fieldToFile(field); break;
	case 8:  distance = 7 - fieldToRank(field); break;
	case -8: distance = fieldToRank(field); break;
	case 9:  distance = min(7 - fieldToFile(field), 7 - fieldToRank(field)); break;
	case -7: distance = min(7 - fieldToFile(field), fieldToRank(field)); break;
	case -9: distance = min(fieldToFile(field), fieldToRank(field)); break;
	case 7:  distance = min(fieldToFile(field), 7 - fieldToRank(field)); break;
	default: distance = -1; break;
	}

	assert(distance != -1);
	return distance;
}

int FastMagicBitboards::fieldToFile(int field)
{
	assert(field >= 0 && field < 64);
	return field % 8;
}

int FastMagicBitboards::fieldToRank(int field)
{
	assert(field >= 0 && field < 64);
	return field / 8;
}

U64 FastMagicBitboards::randU64()
{
	return _randomDistribution(_randomGenerator);
}