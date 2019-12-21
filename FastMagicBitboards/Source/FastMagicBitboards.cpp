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

void FastMagicBitboards::GenerateForRook(int field)
{
	assert(field >= 0 && field < 64);
	calculateField(field, _rookMagicStructures, rookAttacksGenerator, rookMasksGenerator);
}

void FastMagicBitboards::GenerateForBishop()
{
	for (int i = 0; i < 64; i++)
	{
		GenerateForBishop(i);
	}
}

void FastMagicBitboards::GenerateForBishop(int field)
{
	assert(field >= 0 && field < 64);
	calculateField(field, _bishopMagicStructures, bishopAttacksGenerator, bishopMasksGenerator);
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
	occupancy >>= _rookMagicStructures[field].Shift;
	return _bishopMagicStructures[field].MagicAttacks[occupancy];
}

void FastMagicBitboards::calculateField(int field, std::array<MagicStructure, 64> &pieceMagicStructures, std::unique_ptr<AttacksGeneratorBase> &attacksGenerator, std::unique_ptr<MasksGeneratorBase> &masksGenerator)
{
	assert(field >= 0 && field < 64);
	pieceMagicStructures[field].Mask = masksGenerator->Generate(field);

	int permutationsCount = 1 << BitOperations::Count(pieceMagicStructures[field].Mask);
	auto permutations = std::make_unique<U64 []>(permutationsCount);
	auto attacks = std::make_unique<U64[]>(permutationsCount);

	for (int p = 0; p < permutationsCount; p++)
	{
		permutations[p] = Permutations::Generate(p, field, pieceMagicStructures[field].Mask);
		attacks[p] = attacksGenerator->Generate(field, permutations[p]);
	}

	pieceMagicStructures[field].MagicNumber = generateMagicNumber(pieceMagicStructures[field], permutations, attacks);
}

U64 FastMagicBitboards::generateMagicNumber(MagicStructure &pieceMagicStructures, std::unique_ptr<U64 []> &permutations, std::unique_ptr<U64[]> &attacks)
{
	int attacksCount = 1 << BitOperations::Count(pieceMagicStructures.Mask);
	int shift = BitOperations::Count(pieceMagicStructures.Mask);

	pieceMagicStructures.Shift = 64 - shift;
	pieceMagicStructures.MagicAttacks = std::make_unique<U64[]>((int)(1 << shift));

	bool success = false;
	while (!success)
	{
		U64 magicNumber = randU64() & randU64() & randU64();
		memset(pieceMagicStructures.MagicAttacks.get(), 0, attacksCount * sizeof(U64));

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