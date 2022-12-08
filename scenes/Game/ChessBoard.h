#pragma once

#include <bitset>

class ChessBoard
{
public:
	ChessBoard();
	char** board_to_strarr();
	char*  row_to_str(int row);
	char*  col_to_str(int col);
	char   square_to_char(int row, int col);

	void write_strarr_to_board(char** board);
	void write_str_to_row(char* row);
	void write_str_to_col(char* col);
	void write_ch_to_square(char piece);

	void wipe_board();
	void fen_to_board(char *fen_str);

	typedef char (char_row)[8];

private:
	std::bitset<256> board_raw;
	bool white_turn = true;
	std::bitset<4> castle_rights {"1111"};
	short halfmove_clock = 0;
	short fullmove_number = 0;

	bool board_raw_r(int row, int col, int bit);
	void board_raw_w(int row, int col, int bit, bool val);

	char *read_fen_main(char *start_char, int row = 0, int col = 0);
	char *read_fen_castle(char *castle_str);
	char *read_fen_enp(char *enp_str);

	void write_bitset_to_square(const std::bitset<3> *piece, int row, int col, bool is_white);

	const std::bitset<3> king   {"111"};
	const std::bitset<3> queen  {"110"};
	const std::bitset<3> rook   {"101"};
	const std::bitset<3> bishop {"100"};
	const std::bitset<3> knight {"011"};
	const std::bitset<3> pawn   {"010"};
	const std::bitset<3> pawn_e {"001"};
	const std::bitset<3> none   {"000"};

	enum piece {
		NONE   = 0,
		PAWN_E = 1,
		PAWN   = 2,
		KNIGHT = 3,
		BISHOP = 4,
		ROOK   = 5,
		QUEEN  = 6,
		KING   = 7,
	};

};
