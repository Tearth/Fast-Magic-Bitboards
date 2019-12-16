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
}

void FastMagicBitboards::generateMasks()
{
	for (int i = 0; i < 64; i++)
	{
		_rookMagicStructures[i].Mask = generateMaskForRook(i);
		_bishopMagicStructures[i].Mask = generateMaskForBishop(i);
	}
}

U64 FastMagicBitboards::generateMaskForRook(int field)
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

U64 FastMagicBitboards::generateMaskForBishop(int field)
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