#ifndef BIT_OPERATIONS_HPP
#define BIT_OPERATIONS_HPP

#include <assert.h>

#ifdef _MSC_VER
#include <immintrin.h>
#include <intrin.h>
#endif

#include "Typedefs.hpp"

class BitOperations
{
public:
	static U64 GetLSB(U64 number);
	static int BitScan(U64 number);
	static U64 PopLSB(U64 number);
	static int Count(U64 number);

private:
	BitOperations();
};

#endif