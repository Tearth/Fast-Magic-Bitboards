#include "BishopMasksGenerator.hpp"

U64 BishopMasksGenerator::Generate(int field)
{
	assert(field >= 0 && field < 64);

	U64 mask = 0;
	mask |= generateMaskForDirection(field + 7, 7);
	mask |= generateMaskForDirection(field - 7, -7);
	mask |= generateMaskForDirection(field + 9, 9);
	mask |= generateMaskForDirection(field - 9, -9);

	return mask;
}

U64 BishopMasksGenerator::generateMaskForDirection(int field, int shift)
{
	assert(shift != 0);

	U64 mask = 0;
	for (int i = BitboardOperations::DistanceToEdge(field, shift); i >= 0; i--)
	{
		mask |= (U64)1 << field;
		field += shift;
	}

	return mask;
}