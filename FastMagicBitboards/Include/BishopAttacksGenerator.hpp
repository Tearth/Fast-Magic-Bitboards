#ifndef BISHOP_ATTACKS_GENERATOR_HPP
#define BISHOP_ATTACKS_GENERATOR_HPP

#include <assert.h>
#include "AttacksGeneratorBase.hpp"

class BishopAttacksGenerator : public AttacksGeneratorBase
{
public:
	U64 Generate(int field, U64 occupancy);
};

#endif