#include "BitboardOperations.hpp"

int BitboardOperations::DistanceToEdge(int field, int shift)
{
	assert(shift != 0);

	if (field < 0 || field >= 64)
	{
		return -1;
	}

	int distance;
	switch (shift)
	{
	case 1:  distance = 7 - FieldToFile(field); break;
	case -1: distance = FieldToFile(field); break;
	case 8:  distance = 7 - FieldToRank(field); break;
	case -8: distance = FieldToRank(field); break;
	case 9:  distance = std::min(7 - FieldToFile(field), 7 - FieldToRank(field)); break;
	case -7: distance = std::min(7 - FieldToFile(field), FieldToRank(field)); break;
	case -9: distance = std::min(FieldToFile(field), FieldToRank(field)); break;
	case 7:  distance = std::min(FieldToFile(field), 7 - FieldToRank(field)); break;
	default: distance = -1; break;
	}

	assert(distance != -1);
	return distance;
}

int BitboardOperations::FieldToFile(int field)
{
	assert(field >= 0 && field < 64);
	return field % 8;
}

int BitboardOperations::FieldToRank(int field)
{
	assert(field >= 0 && field < 64);
	return field / 8;
}