#ifndef FAST_MAGIC_BITBOARDS_HPP
#define FAST_MAGIC_BITBOARDS_HPP

#include <stdint.h>

typedef uint64_t U64, Bitboard;

class FastMagicBitboards
{
public:
	FastMagicBitboards();
	~FastMagicBitboards();

	void Generate();
	
private:
	Bitboard _rookAttacks[64][512];
	Bitboard _bishopAttacks[64][512];
	U64 _rookMagicNumbers[64];
	U64 _bishopMagicNumbers[64];
};

#endif