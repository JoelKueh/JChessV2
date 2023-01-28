#pragma once

#include <bitset>
#include "../../Global.h"

class ChessBoard
{
public:
	ChessBoard();
	bool white_turn;
	short halfmove_clock = 0;
	short fullmove_number = 0;
	char** board_to_strarr();

	void write_strarr_to_board(char** board);
	void write_str_to_row(char* row);
	void write_str_to_col(char* col);
	void write_ch_to_square(char piece);

	void wipe_board();
	void fen_to_board(char *fen_str);

	typedef char (char_row)[8];

private:
	std::bitset<256> board_raw;
	std::bitset<4> castle_rights {"1111"};

	bool board_raw_r(int row, int col, int bit);
	void board_raw_w(int row, int col, int bit, bool val);

	char *read_fen_main(char *start_char, int row = 0, int col = 0);
	char *read_fen_castle(char *castle_str);
	char *read_fen_enp(char *enp_str);

	void write_bitset_to_square(const std::bitset<3> *piece, int row, int col, bool is_white);
};