#ifndef FAST_MAGIC_BITBOARDS_HPP
#define FAST_MAGIC_BITBOARDS_HPP

#include <algorithm>
#include "Typedefs.hpp"
#include "MagicStructure.hpp"
#include "BitOperations.hpp"

using namespace std;

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
	U64 generateAttacksForDirection(int field, int shift, U64 occupancy);
	int distanceToEdge(int field, int shift);

	int fieldToFile(int field);
	int fieldToRank(int field);

	Bitboard *_rookAttacks[64];
	Bitboard *_bishopAttacks[64];
	MagicStructure _rookMagicStructures[64];
	MagicStructure _bishopMagicStructures[64];
};

#endif