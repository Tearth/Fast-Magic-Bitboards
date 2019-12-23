#include "FastMagicBitboards.hpp"

FastMagicBitboards::FastMagicBitboards() : _randomGenerator(_randomDevice()), _randomDistribution(0, UINT64_MAX)
{
	rookMasksGenerator = std::make_unique<RookMasksGenerator>();
	bishopMasksGenerator = std::make_unique<BishopMasksGenerator>();

	rookAttacksGenerator = std::make_unique<RookAttacksGenerator>();
	bishopAttacksGenerator = std::make_unique<BishopAttacksGenerator>();
}

void FastMagicBitboards::GenerateForRook()
{
	for (int i = 0; i < 64; i++)
	{
		GenerateForRook(i);
	}
}

U64 FastMagicBitboards::GenerateForRook(int field)
{
	assert(field >= 0 && field < 64);

	static int defaultRookShifts[64] =
	{
		12, 11, 11, 11, 11, 11, 11, 12,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		11, 10, 10, 10, 10, 10, 10, 11,
		12, 11, 11, 11, 11, 11, 11, 12
	};

	return calculateField(field, defaultRookShifts[field], _rookMagicStructures[field], rookAttacksGenerator, rookMasksGenerator);
}

U64 FastMagicBitboards::GenerateForRook(int field, int shift)
{
	assert(field >= 0 && field < 64);
	return calculateField(field, shift, _rookMagicStructures[field], rookAttacksGenerator, rookMasksGenerator);
}

void FastMagicBitboards::GenerateForBishop()
{
	for (int i = 0; i < 64; i++)
	{
		GenerateForBishop(i);
	}
}

U64 FastMagicBitboards::GenerateForBishop(int field)
{
	assert(field >= 0 && field < 64);

	static int defaultBishopShifts[64] =
	{
		6, 5, 5, 5, 5, 5, 5, 6,
		5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 7, 7, 7, 7, 5, 5,
		5, 5, 7, 9, 9, 7, 5, 5,
		5, 5, 7, 9, 9, 7, 5, 5,
		5, 5, 7, 7, 7, 7, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5,
		6, 5, 5, 5, 5, 5, 5, 6
	};

	return calculateField(field, defaultBishopShifts[field], _bishopMagicStructures[field], bishopAttacksGenerator, bishopMasksGenerator);
}

U64 FastMagicBitboards::GenerateForBishop(int field, int shift)
{
	assert(field >= 0 && field < 64);
	return calculateField(field, shift, _bishopMagicStructures[field], bishopAttacksGenerator, bishopMasksGenerator);
}

U64 FastMagicBitboards::GetRookAttacks(int field, U64 occupancy)
{
	assert(field >= 0 && field < 64);

	occupancy &= _rookMagicStructures[field].Mask;
	occupancy *= _rookMagicStructures[field].MagicNumber;
	occupancy >>= _rookMagicStructures[field].Shift;
	return _rookMagicStructures[field].MagicAttacks[occupancy];
}

U64 FastMagicBitboards::GetBishopAttacks(int field, U64 occupancy)
{
	assert(field >= 0 && field < 64);

	occupancy &= _bishopMagicStructures[field].Mask;
	occupancy *= _bishopMagicStructures[field].MagicNumber;
	occupancy >>= _bishopMagicStructures[field].Shift;
	return _bishopMagicStructures[field].MagicAttacks[occupancy];
}

std::array<PersistentMagicStructure, 128> FastMagicBitboards::GetMagicStructures()
{
	std::array<PersistentMagicStructure, 128> persistentMagicStructures;
	for (int i = 0; i < 64; i++)
	{
		persistentMagicStructures[i].MagicNumber = _rookMagicStructures[i].MagicNumber;
		persistentMagicStructures[i].Shift = _rookMagicStructures[i].Shift;

		persistentMagicStructures[i + 64].MagicNumber = _bishopMagicStructures[i].MagicNumber;
		persistentMagicStructures[i + 64].Shift = _bishopMagicStructures[i].Shift;
	}

	return persistentMagicStructures;
}

bool FastMagicBitboards::SetMagicStructures(std::array<PersistentMagicStructure, 128> persistentMagicStructures)
{
	for (int i = 0; i < 64; i++)
	{
		if (!calculateField(i, persistentMagicStructures[i], _rookMagicStructures[i], rookAttacksGenerator, rookMasksGenerator) ||
			!calculateField(i, persistentMagicStructures[i + 64], _bishopMagicStructures[i], bishopAttacksGenerator, bishopMasksGenerator))
		{
			return false;
		}
	}

	return true;
}

U64 FastMagicBitboards::calculateField(int field, int shift, MagicStructure &pieceMagicStructures, std::unique_ptr<AttacksGeneratorBase> &attacksGenerator, std::unique_ptr<MasksGeneratorBase> &masksGenerator)
{
	pieceMagicStructures.Mask = masksGenerator->Generate(field);

	int permutationsCount = 1 << BitOperations::Count(pieceMagicStructures.Mask);
	auto permutations = std::make_unique<U64 []>(permutationsCount);
	auto attacks = std::make_unique<U64[]>(permutationsCount);

	for (int p = 0; p < permutationsCount; p++)
	{
		permutations[p] = Permutations::Generate(p, field, pieceMagicStructures.Mask);
		attacks[p] = attacksGenerator->Generate(field, permutations[p]);
	}

	pieceMagicStructures.MagicNumber = generateMagicNumber(shift, pieceMagicStructures, permutations, attacks);
	return pieceMagicStructures.MagicNumber;
}

bool FastMagicBitboards::calculateField(int field, PersistentMagicStructure &persistentMagicStructure, MagicStructure &pieceMagicStructures, std::unique_ptr<AttacksGeneratorBase> &attacksGenerator, std::unique_ptr<MasksGeneratorBase> &masksGenerator)
{
	pieceMagicStructures.Mask = masksGenerator->Generate(field);
	pieceMagicStructures.Shift = persistentMagicStructure.Shift;
	pieceMagicStructures.MagicNumber = persistentMagicStructure.MagicNumber;
	pieceMagicStructures.MagicAttacks = std::make_unique<U64[]>((U64)1 << (64 - persistentMagicStructure.Shift));

	int permutationsCount = 1 << BitOperations::Count(pieceMagicStructures.Mask);
	auto permutations = std::make_unique<U64[]>(permutationsCount);
	auto attacks = std::make_unique<U64[]>(permutationsCount);

	for (int p = 0; p < permutationsCount; p++)
	{
		permutations[p] = Permutations::Generate(p, field, pieceMagicStructures.Mask);
		attacks[p] = attacksGenerator->Generate(field, permutations[p]);
	}

	for (int i = 0; i < permutationsCount; i++)
	{
		U64 hash = permutations[i] * persistentMagicStructure.MagicNumber;
		int index = (int)(hash >> persistentMagicStructure.Shift);

		if (pieceMagicStructures.MagicAttacks[index] != 0 && pieceMagicStructures.MagicAttacks[index] != attacks[i])
		{
			return false;
		}

		pieceMagicStructures.MagicAttacks[index] = attacks[i];
	}

	return true;
}

U64 FastMagicBitboards::generateMagicNumber(int shift, MagicStructure &pieceMagicStructures, std::unique_ptr<U64 []> &permutations, std::unique_ptr<U64[]> &attacks)
{
	int attacksCount = 1 << BitOperations::Count(pieceMagicStructures.Mask);
	int magicAttacksCount = 1 << shift;

	pieceMagicStructures.Shift = 64 - shift;
	pieceMagicStructures.MagicAttacks = std::make_unique<U64[]>(magicAttacksCount);

	bool success = false;
	while (!success)
	{
		U64 magicNumber = randU64() & randU64() & randU64();
		memset(pieceMagicStructures.MagicAttacks.get(), 0, magicAttacksCount * sizeof(U64));

		success = true;
		for (int i = 0; i < attacksCount; i++)
		{
			U64 hash = permutations[i] * magicNumber;
			int index = (int)(hash >> pieceMagicStructures.Shift);

			if (pieceMagicStructures.MagicAttacks[index] != 0 && pieceMagicStructures.MagicAttacks[index] != attacks[i])
			{
				success = false;
				break;
			}

			pieceMagicStructures.MagicAttacks[index] = attacks[i];
		}

		if (success)
		{
			return magicNumber;
		}
	}

	return -1;
}

U64 FastMagicBitboards::randU64()
{
	return _randomDistribution(_randomGenerator);
}