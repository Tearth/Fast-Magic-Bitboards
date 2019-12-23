#include "MasksGeneratorBase.hpp"

U64 MasksGeneratorBase::generateMaskForDirection(int field, int shift)
{
	assert(shift != 0);

	U64 mask = 0;
	for (int i = BitboardOperations::DistanceToEdge(field, shift) - 1; i > 0; i--)
	{
		field += shift;
		mask |= (U64)1 << field;
	}

	return mask;
}