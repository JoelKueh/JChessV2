#include "move_tables.h"

void CB::Tables::gen_move_tables()
{
	if (tables_generated)
		return;
	
	gen_natk();
	gen_katk();
	gen_patk();
	hashtab();
	tables_generated = true;
}

void CB::Tables::gen_natk()
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

uint64_t CB::Tables::read_natk(int sq) {
	return NAtkTable[sq];
}

void CB::Tables::gen_katk()
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

uint64_t CB::Tables::read_katk(int sq) {
	return KAtkTable[sq];
}

void CB::Tables::gen_patk()
{
	for (int j = 0; j < 64; ++j) {
		uint64_t output = 0;
		uint64_t not_rcol = 0xFEFEFEFEFEFEFEFEULL;
		uint64_t not_lcol = 0x7F7F7F7F7F7F7F7FULL;

		// This may overflow, but that should not matter as overflow of a r-shift
		// becomes zero.
		output |= (1ULL << (j - 9)) & not_lcol;
		output |= (1ULL << (j - 7)) & not_rcol;

		PAtkTable[1][j] = output;
	}

	for (int j = 0; j < 64; ++j) {
		uint64_t output = 0;
		uint64_t not_rcol = 0xFEFEFEFEFEFEFEFEULL;
		uint64_t not_lcol = 0x7F7F7F7F7F7F7F7FULL;

		// This may overflow, but that should not matter as overflow of a r-shift
		// becomes zero.
		output |= (1ULL << (j + 9)) & not_rcol;
		output |= (1ULL << (j + 7)) & not_lcol;

		PAtkTable[0][j] = output;
	}
}

uint64_t CB::Tables::read_patk(bool is_white, int sq)
{
	return PAtkTable[is_white][sq];
}
