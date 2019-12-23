#include <iostream>
#include <chrono>
#include <string>
#include <FastMagicBitboards.hpp>

using namespace std;
using namespace std::chrono;

void testRook(int field, U64 occupacion, U64 expectedResult);
void testBishop(int field, U64 occupacion, U64 expectedResult);
void printError(string pieceName, int field, U64 occupacion, U64 result, U64 expectedResult);

FastMagicBitboards fastBitboards;

int main()
{
	while (1)
	{
		auto start = chrono::steady_clock::now();
		fastBitboards.GenerateForRook();
		fastBitboards.GenerateForBishop();
		auto end = chrono::steady_clock::now();

		cout << "GenTime: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

		testRook(0, 0xc19da1890c182089, 0x10101010e);
		testRook(35, 0xc19da1890c182089, 0x808f708000000);
		testRook(52, 0xc19da1890c182089, 0x10e8101010100000);
		testRook(63, 0xc19da1890c182089, 0x4080000000000000);

		testBishop(0, 0xc19da1890c182089, 0x8040200);
		testBishop(35, 0xc19da1890c182089, 0x4122140014204080);
		testBishop(52, 0xc19da1890c182089, 0x2800280402010000);
		testBishop(63, 0xc19da1890c182089, 0x40200000000000);
	}
}

void testRook(int field, U64 occupacion, U64 expectedResult)
{
	U64 result = fastBitboards.GetRookAttacks(field, occupacion);
	if (result != expectedResult)
	{
		printError("rook", field, occupacion, result, expectedResult);
	}
}

void testBishop(int field, U64 occupacion, U64 expectedResult)
{
	U64 result = fastBitboards.GetBishopAttacks(field, occupacion);
	if (result != expectedResult)
	{
		printError("bishop", field, occupacion, result, expectedResult);
	}
}

void printError(string pieceName, int field, U64 occupacion, U64 result, U64 expectedResult)
{
	cout << "Invalid result for " << pieceName << ": field = " << field << ", occupacion = " << occupacion << endl;
	cout << "  - result = " << result << endl;
	cout << "  - expected result = " << expectedResult << endl;
}