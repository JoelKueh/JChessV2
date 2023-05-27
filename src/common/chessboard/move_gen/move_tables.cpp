#include "move_tables.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0') << \
  (byte & 0x40 ? '1' : '0') << \
  (byte & 0x20 ? '1' : '0') << \
  (byte & 0x10 ? '1' : '0') << \
  (byte & 0x08 ? '1' : '0') << \
  (byte & 0x04 ? '1' : '0') << \
  (byte & 0x02 ? '1' : '0') << \
  (byte & 0x01 ? '1' : '0') 


#define U64_TO_BB(file, u64)  \
  file << BYTE_TO_BINARY(u64) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 8) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 16) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 24) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 32) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 40) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 48) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 56) << std::endl << std::endl; \
/////////////////////////////

void ChessBoard::MoveTables::gen_move_tables()
{
	if (tables_generated)
		return;
	
	gen_natk();
	gen_katk();
	hashtab();
	tables_generated = true;
}

void ChessBoard::MoveTables::gen_natk()
{
	int sq_offset[8] = {-17, -15, -10, -6, 6, 10, 15, 17};

	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 8; ++j) {
			int sq = i + sq_offset[j];

			if (sq < 0 || sq >= 64)
				continue;

			NAtkTable[i] |= (1ULL << sq);
		}
		if ((1ULL << i) & right_two_cols)
			NAtkTable[i] &= ~left_two_cols;

		if ((1ULL << i) & left_two_cols)
			NAtkTable[i] &= ~right_two_cols;
	}
}

uint64_t ChessBoard::MoveTables::read_natk(int sq) {
	return NAtkTable[sq];
}

void ChessBoard::MoveTables::gen_katk()
{
	
	int sq_offset[8] = { 9, 8, 7, 1, -1, -7, -8, -9};

	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 8; ++j) {
			int sq = i - sq_offset[j];

			if (sq < 0 || sq >= 64)
				continue;
			
			KAtkTable[i] |= (1ULL << sq);
		}

		if ((1ULL << i) & right_two_cols)
			KAtkTable[i] &= ~left_two_cols;

		if ((1ULL << i) & left_two_cols)
			KAtkTable[i] &= ~right_two_cols;
	}
}

uint64_t ChessBoard::MoveTables::read_katk(int sq) {
	return KAtkTable[sq];
}