#ifndef ROOK_ATTACKS_GENERATOR_HPP
#define ROOK_ATTACKS_GENERATOR_HPP

#include <assert.h>
#include "AttacksGeneratorBase.hpp"

class RookAttacksGenerator : public AttacksGeneratorBase
{
public:
	U64 Generate(int field, U64 occupancy);
};

#endif