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