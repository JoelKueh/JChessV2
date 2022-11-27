#include "ChessBoard.h"

// DEBUG
#include <iostream>
#include <ncurses.h>

ChessBoard::ChessBoard()
{
	char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR\0";
	fen_to_board(init_board);
};

char **ChessBoard::board_to_strarr()
{
	char **output;
	output = new char*[8];
	for (int row = 0; row < 8; row++)
	{
		output[row] = new char[8];

		for (int col = 0; col < 8; col++)
		{
			output[row][col] = 'n';
		}
	}

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			std::bitset<4> piece;
			for (int bit = 1; bit < 5; bit++)
			{
				piece[bit] = board_raw_read(row,col,bit);
			}

			switch (piece.to_ulong())
			{
				case 1: output[row][col] = 'p'; break;
				case 0: output[row][col] = ' '; break;
				case 2: output[row][col] = 'p'; break;
				case 3: output[row][col] = 'k'; break;
				case 4: output[row][col] = 'b'; break;
				case 5: output[row][col] = 'r'; break;
				case 6: output[row][col] = 'r'; break;
				case 7: output[row][col] = 'q'; break;
				case 8: output[row][col] = 'k'; break;
				case 9: output[row][col] = 'k'; break;
				default: output[row][col] = 0; break;
			}

			if (board_raw_read(row,col,0) == 1)
			{
				output[row][col] = toupper(output[row][col]);
			}
		}
	}

	return output;
}

// TODO: IMPLEMENT SUPPORT FOR TURN, CASLTES, ENPASSANT, AND 50 MOVE RULE
bool ChessBoard::fen_to_board(char *fen_str)
{
	int row = 0;
	int col = 7;

	for (int i = 0; i <= 87; i++)
	{
		char this_char = fen_str[i];
		if (isdigit(this_char))
		{
			// As the Unicode values for ascii characters are right next to eachother, the Unicode for '0' can
			// be subtracted from the number to get the value in integer form.
			col -= this_char - '0';
		}
		else if (this_char == '/')
		{
			col = 7;
			row++;
		}
		else if (this_char == '\0')
		{
			return true;
		}
		else
		{
			bool is_white = false;
			if (isupper(this_char))
			{
				is_white = true;
			}
			this_char = tolower(this_char);

			switch (this_char)
			{
				case 'k': write_bitset_to_square(&king,   row, col, is_white); break;
				case 'q': write_bitset_to_square(&queen,  row, col, is_white); break;
				case 'r': write_bitset_to_square(&rook,   row, col, is_white); break;
				case 'b': write_bitset_to_square(&bishop, row, col, is_white); break;
				case 'n': write_bitset_to_square(&knight, row, col, is_white); break;
				case 'p': write_bitset_to_square(&pawn,   row, col, is_white); break;
			}
			col--;
		}
	}
	return false;
}

void ChessBoard::write_bitset_to_square(const std::bitset<4> *piece, int row, int col, bool is_white)
{
	for (int i = 1; i < 5; i++)
	{
		board_raw_w(row, col, i, (*piece)[i]);
	}
	board_raw_w(row, col, 0, is_white);
}

bool ChessBoard::board_raw_r(int row, int col, int bit)
{
	return board_raw[row*8*5+col*5+bit];
}

void ChessBoard::board_raw_w(int row, int col, int bit, bool val)
{
	board_raw.set(row*8*5+col*5+bit,val);
}