#include "MasksGeneratorBase.hpp"

U64 MasksGeneratorBase::generateMaskForDirection(int field, int shift)
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