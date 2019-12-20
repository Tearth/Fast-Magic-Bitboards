#ifndef ATTACKS_GENERATOR_BASE_HPP
#define ATTACKS_GENERATOR_BASE_HPP

#include <assert.h>
#include "Typedefs.hpp"
#include "BitboardOperations.hpp"

class AttacksGeneratorBase
{
public:
	virtual U64 Generate(int field, U64 occupancy) = 0;

protected:
	U64 generateAttacksForDirection(int field, int shift, U64 occupancy);
};

#endif