#pragma once
#include <cstdint>

namespace ChessBoard {

namespace Fields {

const uint64_t right_col = 0x8080808080808080;
const uint64_t left_col = 0x0101010101010101;
const uint64_t bottom_row = 0xFF;
const uint64_t top_row  = 0xFF00000000000000;
const int dir_mapping[] = {1, 9, 8, 7, -1, -9, -8, -7};
const uint64_t wrap_stop[] = {right_col, right_col | top_row,
	top_row, left_col | top_row, left_col, left_col | bottom_row,
	bottom_row, right_col | bottom_row};
static uint64_t dir_table[64][8];

void gen_dir_table();
uint64_t gen_dir(int sq, int dir);
uint64_t read_dir_table(int sq, int dir);

}

}