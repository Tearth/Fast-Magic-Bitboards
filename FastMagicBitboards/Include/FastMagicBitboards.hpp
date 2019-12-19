#ifndef FAST_MAGIC_BITBOARDS_HPP
#define FAST_MAGIC_BITBOARDS_HPP

#include <iostream>
#include <random>
#include <array>
#include <algorithm>
#include "Typedefs.hpp"
#include "MagicStructure.hpp"
#include "BitOperations.hpp"
#include "BitboardOperations.hpp"
#include "AttacksGeneratorBase.hpp"
#include "RookAttacksGenerator.hpp"
#include "BishopAttacksGenerator.hpp"

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
	void calculateField(int field, std::array<MagicStructure, 64> &pieceMagicStructures, unique_ptr<AttacksGeneratorBase> &attacksGenerator);
	U64 generatePermutation(int permutationIndex, int field, U64 mask);
	U64 generateMagicNumber(MagicStructure &pieceMagicStructures, std::unique_ptr<U64[]> &permutations, std::unique_ptr<U64[]> &attacks);

	U64 randU64();

	std::array<MagicStructure, 64> _rookMagicStructures;
	std::array<MagicStructure, 64> _bishopMagicStructures;

	unique_ptr<AttacksGeneratorBase> rookAttacksGenerator;
	unique_ptr<AttacksGeneratorBase> bishopAttacksGenerator;

	std::random_device _randomDevice;
	std::default_random_engine _randomGenerator;
	std::uniform_int_distribution<U64> _randomDistribution;
};

#endif