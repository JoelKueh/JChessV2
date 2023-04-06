#include "dir_table.h"

void ChessBoard::Fields::gen_dir_table()
{
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 8; ++j) {
			dir_table[i][j] = gen_dir(i, j);
		}
	}
}

uint64_t ChessBoard::Fields::gen_dir(int sq, int dir)
{
	uint64_t output = 0;
	uint64_t mask = (1ULL << sq);
	while (!(mask & wrap_stop[dir])) {
		mask <<= dir_mapping[dir];
		output |= mask;
	}
	return output;
}

/// @brief Reads an entry from the direction table.
/// @param sq The starting square of the ray (not included in output).
/// @param dir The direction of travel. Counterclockwise 0-7 from right.
/// @return Ray given a starting square and a direction
uint64_t ChessBoard::Fields::read_dir_table(int sq, int dir)
{
	return dir_table[sq][dir];
}