#include "Permutations.hpp"

U64 Permutations::Generate(int permutationIndex, int field, U64 mask)
{
	assert(permutationIndex >= 0);
	assert(field >= 0 && field < 64);

	U64 permutation = 0;
	while (mask != 0)
	{
		U64 lsb = BitOperations::GetLSB(mask);
		mask = BitOperations::PopLSB(mask);

		if ((permutationIndex & 1) != 0)
		{
			int lsbIndex = BitOperations::BitScan(lsb);
			permutation |= (U64)1 << lsbIndex;
		}

		permutationIndex >>= 1;
	}

	return permutation;
}