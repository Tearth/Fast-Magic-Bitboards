#ifndef FAST_MAGIC_BITBOARDS_HPP
#define FAST_MAGIC_BITBOARDS_HPP

#include "Typedefs.hpp"
#include "MagicStructure.hpp"

class FastMagicBitboards
{
public:
	FastMagicBitboards();
	~FastMagicBitboards();

	void Generate();
	
private:
	void generateMasks();
	U64 generateMaskForRook(int field);
	U64 generateMaskForBishop(int field);
	U64 generateMaskForDirection(int field, int shift);

	Bitboard _rookAttacks[64][512];
	Bitboard _bishopAttacks[64][512];
	MagicStructure _rookMagicStructures[64];
	MagicStructure _bishopMagicStructures[64];
};

#endif