#include "BitOperations.hpp"

U64 BitOperations::GetLSB(U64 number)
{
	assert(number != 0);
#ifdef _MSC_VER
	return _blsi_u64(number);
#else
	return number & -(int64_t)number;
#endif
}

int BitOperations::BitScan(U64 number)
{
	assert(number != 0);
#ifdef _MSC_VER
	unsigned long index;

	_BitScanReverse64(&index, number);
	return (int)index;
#else
	static const int index64[64] =
	{
		0,  1, 48,  2, 57, 49, 28,  3,
	   61, 58, 50, 42, 38, 29, 17,  4,
	   62, 55, 59, 36, 53, 51, 43, 22,
	   45, 39, 33, 30, 24, 18, 12,  5,
	   63, 47, 56, 27, 60, 41, 37, 16,
	   54, 35, 52, 21, 44, 32, 23, 11,
	   46, 26, 40, 15, 34, 20, 31, 10,
	   25, 14, 19,  9, 13,  8,  7,  6
	};

	const U64 debruijn64 = (U64)0x03f79d71b4cb0a89;
	return index64[((number & -(int64_t)number) * debruijn64) >> 58];
#endif
}

U64 BitOperations::PopLSB(U64 number)
{
	assert(number != 0);
#ifdef _MSC_VER
	return _blsr_u64(number);
#else
	return number & (number - 1);
#endif
}

int BitOperations::Count(U64 number)
{
#ifdef _MSC_VER
	return (int)_mm_popcnt_u64(number);
#else
	int count = 0;
	while (number)
	{
		number = BitOperations::PopLSB(number);
		count++;
	}

	return count;
#endif
}