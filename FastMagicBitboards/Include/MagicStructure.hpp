#ifndef MAGIC_STRUCTURE_HPP
#define MAGIC_STRUCTURE_HPP

#include "Typedefs.hpp"

struct MagicStructure
{
	U64 Mask;
	U64 MagicNumber;
	std::unique_ptr<U64[]> MagicAttacks;
	int Shift;
};

#endif