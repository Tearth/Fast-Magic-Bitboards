#include "FastMagicBitboards.hpp"

FastMagicBitboards::FastMagicBitboards()
{

}

FastMagicBitboards::~FastMagicBitboards()
{

}

void FastMagicBitboards::Generate()
{
	generateMasks();
	generateAttacks();
}

void FastMagicBitboards::generateMasks()
{
	for (int i = 0; i < 64; i++)
	{
		_rookMagicStructures[i].Mask = generateRookMask(i);
		_rookMagicStructures[i].Offset = BitOperations::Count(_rookMagicStructures[i].Mask);
		_bishopMagicStructures[i].Mask = generateBishopMask(i);
		_bishopMagicStructures[i].Offset = BitOperations::Count(_bishopMagicStructures[i].Mask);
	}
}

U64 FastMagicBitboards::generateRookMask(int field)
{
	const U64 firstFile = 0x101010101010101;
	const U64 firstRank = 0xff;
	const U64 bitboardWithoutFirstAndLastFile = 0x7e7e7e7e7e7e7e7e;
	const U64 bitboardWithoutFirstAndLastRank = 0xffffffffffff00;

	U64 mask = 0;
	mask |= (firstFile << (field % 8)) & bitboardWithoutFirstAndLastRank;
	mask |= (firstRank << ((field / 8) * 8)) & bitboardWithoutFirstAndLastFile;
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

void FastMagicBitboards::generateAttacks()
{
	for (int i = 0; i < 64; i++)
	{
		int permutationsCount = 1 << _rookMagicStructures[i].Offset;
		for (int p = 0; p < permutationsCount; p++)
		{
			U64 rookPermutation = generatePermutation(p, i, _rookMagicStructures[i].Mask);
		}
	}
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