#pragma once

#include <bitset>
#define board_raw(row,col,bit) board_raw[(row)*8*5+(col)*5+(bit)]

#define NONE 0
#define PAWN_C 1
#define PAWN 2
#define KNIGHT 3
#define BISHOP 4
#define ROOK_C 5
#define ROOK 6
#define QUEEN 7
#define KING_C 8
#define KING 9

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
	void write_char_to_square(char piece);

	typedef char (char_row)[8];

private:
	std::bitset<320> board_raw;

	const std::bitset<5> king     {"1001"};
	const std::bitset<5> king_c   {"1000"};
	const std::bitset<5> queen    {"0111"};
	const std::bitset<5> rook     {"0110"};
	const std::bitset<5> rook_c   {"0101"};
	const std::bitset<5> bishop   {"0100"};
	const std::bitset<5> knight   {"0011"};
	const std::bitset<5> pawn     {"0010"};
	const std::bitset<5> pawn_c   {"0001"};
	const std::bitset<5> none     {"0000"};
};
