#ifndef FAST_MAGIC_BITBOARDS_HPP
#define FAST_MAGIC_BITBOARDS_HPP

#include <iostream>
#include <algorithm>
#include <random>
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
	void GenerateForRook(int field);
	void GenerateForBishop(int field);
private:
	U64 generateRookMask(int field);
	U64 generateBishopMask(int field);
	U64 generateMaskForDirection(int field, int shift);
	void generateAttacks(int field, MagicStructure *pieceMagicStructures, Bitboard **pieceAttacks, U64 (FastMagicBitboards::*attacksGenerator)(int, U64));
	U64 generatePermutation(int permutationIndex, int field, U64 mask);
	U64 generateRookAttacks(int field, U64 occupancy);
	U64 generateBishopAttacks(int field, U64 occupancy);
	U64 generateAttacksForDirection(int field, int shift, U64 occupancy);
	U64 generateMagicNumber(MagicStructure *pieceMagicStructures, Bitboard *pieceMagicAttacks, U64 *permutations, Bitboard *attacks);

	int distanceToEdge(int field, int shift);
	int fieldToFile(int field);
	int fieldToRank(int field);
	U64 randU64();

	Bitboard *_rookMagicAttacks[64];
	Bitboard *_bishopMagicAttacks[64];
	MagicStructure _rookMagicStructures[64];
	MagicStructure _bishopMagicStructures[64];
};

#endif