#include <iostream>
#include <FastMagicBitboards.hpp>

int main()
{
	auto fastMagicBitboards = new FastMagicBitboards();
	auto classSizeKilobytes = sizeof(FastMagicBitboards) / 1024;

	fastMagicBitboards->Generate();
}