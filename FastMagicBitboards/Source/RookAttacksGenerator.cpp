#include "RookAttacksGenerator.hpp"

U64 RookAttacksGenerator::Generate(int field, U64 occupancy)
{
	assert(field >= 0 && field < 64);

	U64 attacks = 0;
	attacks |= generateAttacksForDirection(field, 1, occupancy);
	attacks |= generateAttacksForDirection(field, -1, occupancy);
	attacks |= generateAttacksForDirection(field, 8, occupancy);
	attacks |= generateAttacksForDirection(field, -8, occupancy);

	return attacks;
}