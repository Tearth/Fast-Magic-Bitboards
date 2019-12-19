#include <iostream>
#include <chrono>
#include <FastMagicBitboards.hpp>
#include <BitOperations.hpp>

using namespace std;
using namespace std::chrono;

int main()
{
	FastMagicBitboards *fastBitboards = new FastMagicBitboards();

	while (1)
	{
		auto start = chrono::steady_clock::now();
		fastBitboards->GenerateForRook();
		fastBitboards->GenerateForBishop();
		auto end = chrono::steady_clock::now();

		cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
	}
}