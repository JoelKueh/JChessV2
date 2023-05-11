#include <iostream>
#include <cstdint>
#include "chessboard/tf_table.h"

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
  file << BYTE_TO_BINARY(u64 >> 56) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 48) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 40) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 32) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 24) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 16) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 8) << std::endl \
    << BYTE_TO_BINARY(u64) << std::endl << std::endl; \
/////////////////////////////

const int BitTable[64] = {
	63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
	51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
	26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
	58, 20, 37, 17, 36, 8
};

int pop_1st_bit(uint64_t *bb) {
	uint64_t b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return BitTable[(fold * 0x783a9b23) >> 26];
}

int main() {
  uint64_t m = 0x8000000000000000;
  std::cout << pop_1st_bit(&m) << " | " << m << std::endl;
  U64_TO_BB(std::cout, ChessBoard::Fields::tf_table[30][30]);
  return 0;
}
