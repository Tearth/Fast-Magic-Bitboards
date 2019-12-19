#ifndef BITBOARD_OPERATIONS_HPP
#define BITBOARD_OPERATIONS_HPP

#include <assert.h>
#include <algorithm>

using namespace std;

class BitboardOperations
{
public:
	static int DistanceToEdge(int field, int shift);
	static int FieldToFile(int field);
	static int FieldToRank(int field);

private:
	BitboardOperations();
};

#endif