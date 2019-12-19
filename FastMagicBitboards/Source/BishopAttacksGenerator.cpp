#include "BishopAttacksGenerator.hpp"

U64 BishopAttacksGenerator::Generate(int field, U64 occupancy)
{
	assert(field >= 0 && field < 64);

	U64 attacks = 0;
	attacks |= generateAttacksForDirection(field, 7, occupancy);
	attacks |= generateAttacksForDirection(field, -7, occupancy);
	attacks |= generateAttacksForDirection(field, 9, occupancy);
	attacks |= generateAttacksForDirection(field, -9, occupancy);

	return attacks;
}