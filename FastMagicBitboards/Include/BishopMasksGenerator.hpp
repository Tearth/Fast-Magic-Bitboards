#ifndef BISHOP_MASKS_GENERATOR_HPP
#define BISHOP_MASKS_GENERATOR_HPP

#include <assert.h>
#include "MasksGeneratorBase.hpp"

class BishopMasksGenerator : public MasksGeneratorBase
{
public:
	U64 Generate(int field);
};

#endif