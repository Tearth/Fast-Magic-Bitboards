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

	//! Generates magic numbers for rooks.
	/*!
		Generates magic numbers for rooks and stores them in the internal structure. They can be obtained by
		calling GetMagicStructures method. It usually takes a few hundred milliseconds.
	*/
	void GenerateForRook();

	//! Generates magic numbers for rooks on the specified field.
	/*!
		Generates magic numbers for rooks on the specified field and stores them in the internal structure. They can be obtained by
		calling GetMagicStructures method. It usually takes a few hundred milliseconds.
		\param field Field index for which the magic number will be calculated.
		\return Magic number calculated for the specified field.
	*/
	U64 GenerateForRook(int field);

	//! Generates magic numbers for rooks on the specified field and shift.
	/*!
		Generates magic numbers for rooks on the specified field, shift and stores them in the internal structure. They can be obtained by
		calling GetMagicStructures method. It usually takes a few hundred milliseconds.
		\param field Field index for which the magic number will be calculated.
		\param shift Shift used to prepare attacks table. The smaller value will require a smaller table.
		\return Magic number calculated for the specified field.
	*/
	U64 GenerateForRook(int field, int shift);

	//! Generates magic numbers for bishops.
	/*!
		Generates magic numbers for bishops and stores them in the internal structure. They can be obtained by
		calling GetMagicStructures method. It usually takes a few hundred milliseconds.
	*/
	void GenerateForBishop();

	//! Generates magic numbers for bishops on the specified field.
	/*!
		Generates magic numbers for bishops on the specified field and stores them in the internal structure. They can be obtained by
		calling GetMagicStructures method. It usually takes a few hundred milliseconds.
		\param field Field index for which the magic number will be calculated.
		\return Magic number calculated for the specified field.
	*/
	U64 GenerateForBishop(int field);

	//! Generates magic numbers for bishops on the specified field and shift.
	/*!
		Generates magic numbers for bishops on the specified field, shift and stores them in the internal structure. They can be obtained by
		calling GetMagicStructures method. It usually takes a few hundred milliseconds.
		\param field Field index for which the magic number will be calculated.
		\param shift Shift used to prepare attacks table. The smaller value will require a smaller table.
		\return Magic number calculated for the specified field.
	*/
	U64 GenerateForBishop(int field, int shift);

	//! Retrieves bitboard representing available rook moves for the specified field and occupancy.
	/*!
		Retrieves bitboard representing available rook moves for the specified field and occupancy. Remember that you have to
		call GenerateForRook or SetMagicStructures methods first.
		\param field Field index for which the rook attacks will be retrieved.
		\param occupancy Bitboard containing occupancy for which the attacks will be retrieved.
		\return Bitboard representing available rook moves.
	*/
	U64 GetRookAttacks(int field, U64 occupancy);

	//! Retrieves bitboard representing available bishop moves for the specified field and occupancy.
	/*!
		Retrieves bitboard representing available bishop moves for the specified field and occupancy. Remember that you have to
		call GenerateForBishop or SetMagicStructures methods first.
		\param field Field index for which the rook attacks will be retrieved.
		\param occupancy Bitboard containing occupancy for which the attacks will be retrieved.
		\return Bitboard representing available bishop moves.
	*/
	U64 GetBishopAttacks(int field, U64 occupancy);

	//! Retrieves generated magic numbers and shifts.
	/*!
		Retrieves generated magic keys and stores them into the array with 128 elements (first 64 for rooks and last 64 for bishops).
		\return Array with generated magic numbers and shifts.
	*/
	std::array<PersistentMagicStructure, 128> GetMagicStructures();

	//! Sets magic numbers and shifts.
	/*!
		Sets magic numbers and shifts, and then generates for them tables with attacks for rooks and bishops.
		\param persistentMagicStructures Array with magic numbers and shifts for rooks (first 64 elements) and bishops (last 64 elements).
		\return True if attack tables have been generated with success (there was no any collision), otherwise false.
	*/
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