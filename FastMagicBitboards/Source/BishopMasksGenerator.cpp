#include "BishopMasksGenerator.hpp"

U64 BishopMasksGenerator::Generate(int field)
{
	assert(field >= 0 && field < 64);

	U64 mask = 0;
	mask |= generateMaskForDirection(field, 7);
	mask |= generateMaskForDirection(field, -7);
	mask |= generateMaskForDirection(field, 9);
	mask |= generateMaskForDirection(field, -9);

	return mask;
}