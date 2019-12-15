#include "BitOperations.hpp"

int BitOperations::LSB(U64 number)
{
#ifdef _MSC_VER
	return _blsi_u64(number);
#else
	return number & -(int64_t)number;
#endif
}