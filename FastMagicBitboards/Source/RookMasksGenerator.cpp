#include "RookMasksGenerator.hpp"

U64 RookMasksGenerator::Generate(int field)
{
	assert(field >= 0 && field < 64);

	const U64 firstFile = 0x101010101010101;
	const U64 firstRank = 0xff;
	const U64 bitboardWithoutFirstAndLastFile = 0x7e7e7e7e7e7e7e7e;
	const U64 bitboardWithoutFirstAndLastRank = 0xffffffffffff00;

	U64 mask = 0;
	mask |= (firstFile << BitboardOperations::FieldToFile(field)) & bitboardWithoutFirstAndLastRank;
	mask |= (firstRank << (BitboardOperations::FieldToRank(field) * 8)) & bitboardWithoutFirstAndLastFile;
	mask &= ~((U64)1 << field);

	return mask;
}