#include <iostream>
#include <chrono>
#include <FastMagicBitboards.hpp>
#include <BitOperations.hpp>

using namespace std;
using namespace std::chrono;

volatile U64 trap;
int main()
{
	FastMagicBitboards *fastBitboards = new FastMagicBitboards();

	while (1)
	{
		auto start = chrono::steady_clock::now();
		fastBitboards->GenerateForRook();
		fastBitboards->GenerateForBishop();
		auto end = chrono::steady_clock::now();

		cout << "GenTime: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms, ";

		auto start2 = chrono::steady_clock::now();
		U64 sum = 0;
		for (int i = 0; i < 1000000000; i++)
		{
		//	uint64_t testOccupancy = 0xc0f900050000f834 + i;
		//	sum += fastBitboards->GetRookAttacks(0, testOccupancy);
		}
		auto end2 = chrono::steady_clock::now();
		trap = sum;

		cout << "GetTime: " << chrono::duration_cast<chrono::milliseconds>(end2 - start2).count() << " ms" << endl;
	}
}