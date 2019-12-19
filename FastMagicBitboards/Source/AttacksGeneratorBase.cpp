#include "AttacksGeneratorBase.hpp"

U64 AttacksGeneratorBase::generateAttacksForDirection(int field, int shift, U64 occupancy)
{
	assert(field >= 0 && field < 64);
	assert(shift != 0);

	U64 attacks = 0;
	for (int i = BitboardOperations::DistanceToEdge(field, shift); i > 0 && (((U64)1 << field) & occupancy) == 0; i--)
	{
		field += shift;
		attacks |= (U64)1 << field;
	}

	return attacks;
}