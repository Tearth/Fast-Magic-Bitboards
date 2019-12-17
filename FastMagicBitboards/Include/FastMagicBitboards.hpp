#ifndef FAST_MAGIC_BITBOARDS_HPP
#define FAST_MAGIC_BITBOARDS_HPP

#include "Typedefs.hpp"
#include "MagicStructure.hpp"
#include "BitOperations.hpp"

class FastMagicBitboards
{
public:
	FastMagicBitboards();
	~FastMagicBitboards();

	void Generate();
	
private:
	void generateMasks();
	U64 generateRookMask(int field);
	U64 generateBishopMask(int field);
	U64 generateMaskForDirection(int field, int shift);
	void generateAttacks();
	U64 generatePermutation(int permutationIndex, int field, U64 mask);
	U64 generateRookAttacks(int field, U64 occupancy);
	U64 generateBishopAttacks(int field, U64 occupancy);
	U64 generateAttacksForDirection(int field, int shift);

	Bitboard _rookAttacks[64][512];
	Bitboard _bishopAttacks[64][512];
	MagicStructure _rookMagicStructures[64];
	MagicStructure _bishopMagicStructures[64];
};

#endif