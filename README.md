# Fast Magic Bitboards
Implementation of the [Magic Bitboards algorithm](https://www.chessprogramming.org/Magic_Bitboards) ([fancy](https://www.chessprogramming.org/Magic_Bitboards#Fancy) variation), used in chess engines. Contains functions optimized for MSVC (using [BMI instruction set](https://en.wikipedia.org/wiki/Bit_Manipulation_Instruction_Sets))

## Performance
For i5 8300h CPU, it takes about 400 milliseconds to generate all magic numbers for rooks and bishops, assuming that we use standard shift values.

[BitOperations.cpp](https://github.com/Tearth/Fast-Magic-Bitboards/blob/master/FastMagicBitboards/Source/BitOperations.cpp) file contains a set of functions optimized for fast operations on single bits. Tests have shown that they are nearly two times faster than the straightforward algorithms.

## Available methods
  - `void GenerateForRook()` - generates magic numbers for rooks on all fields.
  - `U64 GenerateForRook(int field)` - generates magic numbers for rooks on the specified field.
  - `U64 GenerateForRook(int field, int shift)` - generates magic numbers for rooks on the specified field, using passed shift.
  - `void GenerateForBishop()` - generates magic numbers for bishops on all fields.
  - `U64 GenerateForBishop(int field)` - generates magic numbers for bishops on the specified field.
  - `U64 GenerateForBishop(int field, int shift)` - generates magic numbers for rooks on the specified field, using passed shift.
  - `U64 GetRookAttacks(int field, U64 occupancy)` - retrieves bitboard representing available rook moves for the specified field and occupancy.
  - `U64 GetBishopAttacks(int field, U64 occupancy)` - retrieves bitboard representing available bishop moves for the specified field and occupancy.
  - `std::array<PersistentMagicStructure, 128> GetMagicStructures()` - retrieves generated magic numbers and shifts.
  - `bool SetMagicStructures(std::array<PersistentMagicStructure, 128> persistentMagicStructures)` - sets magic numbers and shifts.

## Example app
Values passed in the `testRook` and `testBishop` functions have been created using [Bitboard Calculator](http://cinnamonchess.altervista.org/bitboard_calculator/Calc.html).

```cpp
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
```