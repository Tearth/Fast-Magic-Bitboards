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
#include "MasksGeneratorBase.hpp"
#include "RookMasksGenerator.hpp"
#include "BishopMasksGenerator.hpp"
#include "Permutations.hpp"
#include "PersistentMagicStructure.hpp"

class FastMagicBitboards
{
public:
	FastMagicBitboards();

	void GenerateForRook();
	U64 GenerateForRook(int field);
	U64 GenerateForRook(int field, int shift);
	void GenerateForBishop();
	U64 GenerateForBishop(int field);
	U64 GenerateForBishop(int field, int shift);

	U64 GetRookAttacks(int field, U64 occupancy);
	U64 GetBishopAttacks(int field, U64 occupancy);

	std::array<PersistentMagicStructure, 128> GetMagicStructures();
	bool SetMagicStructures(std::array<PersistentMagicStructure, 128> persistentMagicStructures);
private:
	U64 calculateField(int field, int shift, MagicStructure &pieceMagicStructures, std::unique_ptr<AttacksGeneratorBase> &attacksGenerator, std::unique_ptr<MasksGeneratorBase> &masksGenerator);
	bool calculateField(int field, PersistentMagicStructure &persistentMagicStructure, MagicStructure &pieceMagicStructures, std::unique_ptr<AttacksGeneratorBase> &attacksGenerator, std::unique_ptr<MasksGeneratorBase> &masksGenerator);
	U64 generateMagicNumber(int shift, MagicStructure &pieceMagicStructures, std::unique_ptr<U64[]> &permutations, std::unique_ptr<U64[]> &attacks);

	U64 randU64();

	std::array<MagicStructure, 64> _rookMagicStructures;
	std::array<MagicStructure, 64> _bishopMagicStructures;

	std::unique_ptr<MasksGeneratorBase> rookMasksGenerator;
	std::unique_ptr<MasksGeneratorBase> bishopMasksGenerator;

	std::unique_ptr<AttacksGeneratorBase> rookAttacksGenerator;
	std::unique_ptr<AttacksGeneratorBase> bishopAttacksGenerator;

	std::random_device _randomDevice;
	std::mt19937_64 _randomGenerator;
	std::uniform_int_distribution<U64> _randomDistribution;
};

#endif