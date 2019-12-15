#ifndef BIT_OPERATIONS_HPP
#define BIT_OPERATIONS_HPP

#include "Typedefs.hpp"

#ifdef _MSC_VER
#include <immintrin.h>
#endif

static class BitOperations
{
public:
	static int LSB(U64 number);
};

#endif