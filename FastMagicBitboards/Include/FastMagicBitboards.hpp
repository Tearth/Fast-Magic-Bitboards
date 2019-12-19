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

	void GenerateForRook();
	void GenerateForRook(int field);
	void GenerateForBishop();
	void GenerateForBishop(int field);

	Bitboard GetRookAttacks(int field, Bitboard occupancy);
	Bitboard GetBishopAttacks(int field, Bitboard occupancy);
private:
	U64 generateRookMask(int field);
	U64 generateBishopMask(int field);
	U64 generateMaskForDirection(int field, int shift);
	void generateAttacks(int field, MagicStructure *pieceMagicStructures, U64 (FastMagicBitboards::*attacksGenerator)(int, U64));
	U64 generatePermutation(int permutationIndex, int field, U64 mask);
	U64 generateRookAttacks(int field, U64 occupancy);
	U64 generateBishopAttacks(int field, U64 occupancy);
	U64 generateAttacksForDirection(int field, int shift, U64 occupancy);
	U64 generateMagicNumber(MagicStructure *pieceMagicStructures, U64 *permutations, Bitboard *attacks);

	int distanceToEdge(int field, int shift);
	int fieldToFile(int field);
	int fieldToRank(int field);
	U64 randU64();

	MagicStructure _rookMagicStructures[64];
	MagicStructure _bishopMagicStructures[64];

	std::random_device _randomDevice;
	std::default_random_engine _randomGenerator;
	std::uniform_int_distribution<U64> _randomDistribution;
};

#endif