#ifndef BIT_OPERATIONS_HPP
#define BIT_OPERATIONS_HPP

#include <assert.h>

#ifdef _MSC_VER
#include <immintrin.h>
#include <intrin.h>
#endif

#include "Typedefs.hpp"

static class BitOperations
{
public:
	static U64 LSB(U64 number);
	static int BitScan(U64 number);
};

#endif