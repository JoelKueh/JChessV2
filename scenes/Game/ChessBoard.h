#pragma once

#include <bitset>
#define board_raw_read(row,col,bit) board_raw[(row)*8*5+(col)*5+(bit)]
#define board_raw_set(row,col,bit,val) board_raw.set((row)*8*5+(col)*5+(bit),val)

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

	bool fen_to_board(char *fen_str);

	typedef char (char_row)[8];

private:
	std::bitset<320> board_raw;
	bool board_raw_r(int row, int col, int bit);
	void board_raw_w(int row, int col, int bit, bool val);

	void write_bitset_to_square(const std::bitset<4> *piece, int row, int col, bool is_white);
	const std::bitset<4> king     {"1001"};
	const std::bitset<4> king_c   {"1000"};
	const std::bitset<4> queen    {"0111"};
	const std::bitset<4> rook     {"0110"};
	const std::bitset<4> rook_c   {"0101"};
	const std::bitset<4> bishop   {"0100"};
	const std::bitset<4> knight   {"0011"};
	const std::bitset<4> pawn     {"0010"};
	const std::bitset<4> pawn_c   {"0001"};
	const std::bitset<4> none     {"0000"};

	enum piece {
		NONE   = 0,
		PAWN_C = 1,
		PAWN   = 2,
		KNIGHT = 3,
		BISHOP = 4,
		ROOK_C = 5,
		ROOK   = 6,
		QUEEN  = 7,
		KING_C = 8,
		KING   = 9,
	};

};
