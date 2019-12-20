#ifndef MASKS_GENERATOR_BASE_HPP
#define MASKS_GENERATOR_BASE_HPP

#include <assert.h>
#include "Typedefs.hpp"
#include "BitboardOperations.hpp"

class MasksGeneratorBase
{
public:
	virtual U64 Generate(int field) = 0;

protected:
	U64 generateMaskForDirection(int field, int shift);
};

#endif