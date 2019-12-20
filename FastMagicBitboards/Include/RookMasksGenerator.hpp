#ifndef ROOK_MASKS_GENERATOR_HPP
#define ROOK_MASKS_GENERATOR_HPP

#include <assert.h>
#include "MasksGeneratorBase.hpp"

class RookMasksGenerator : public MasksGeneratorBase
{
public:
	U64 Generate(int field);
};

#endif