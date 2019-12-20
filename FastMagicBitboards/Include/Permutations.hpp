#ifndef PERMUTATIONS_HPP
#define PERMUTATIONS_HPP

#include <assert.h>
#include "Typedefs.hpp"
#include "BitOperations.hpp"

class Permutations
{
public:
	static U64 Generate(int permutationIndex, int field, U64 mask);

private:
	Permutations();
};

#endif