#include "ChessBoard.h"

// DEBUG
#include <iostream>
#include <ncurses.h>

ChessBoard::ChessBoard()
{
	char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\0";
	fen_to_board(init_board);
}

char** ChessBoard::board_to_strarr()
{
	char** output = new char*[8];
	for (int row = 0; row < 8; row++)
	{
		output[row] = new char[8];

		for (int col = 0; col < 8; col++)
		{
			output[row][col] = ' ';
		}
	}

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			std::bitset<3> piece;
			for (int bit = 0; bit < 3; bit++)
			{
				piece[bit] = board_raw_r(row, col, bit);
			}

			switch (piece.to_ulong())
			{
				case 0: output[row][col] = ' '; break;
				case 1: output[row][col] = ' '; break;
				case 2: output[row][col] = 'p'; break;
				case 3: output[row][col] = 'k'; break;
				case 4: output[row][col] = 'b'; break;
				case 5: output[row][col] = 'r'; break;
				case 6: output[row][col] = 'q'; break;
				case 7: output[row][col] = 'k'; break;
				default: output[row][col] = 0; break;
			}

			if (board_raw_r(row, col, 3) == 1)
			{
				output[row][col] = toupper(output[row][col]);
			}
		}
	}
	return output;
}

void ChessBoard::fen_to_board(char *fen_str)
{
	wipe_board();

	char *char_pos;
	char_pos = read_fen_main(fen_str);
	
	if (*char_pos == 'w')
	{
		white_turn = true;
	}
	else
	{
		white_turn = false;
	}
	char_pos += 2;

	char_pos = read_fen_castle(char_pos);
	char_pos = read_fen_enp(char_pos);

	halfmove_clock = *char_pos - '0';
	char_pos += 2;
	fullmove_number = *char_pos - '0';
}

void ChessBoard::wipe_board()
{
	for (int i = 0; i < 256; ++i)
	{
		board_raw[i] = false;
	}
	white_turn = true;
	for (int i = 0; i < 4; ++i)
	{
		castle_rights[i] = false;
	}
}

char *ChessBoard::read_fen_main(char *char_pos, int row, int col)
{
	char this_char = *char_pos;
	if (isdigit(this_char))
	{
		// As the Unicode values for ascii characters are right next to eachother, the Unicode for '0' can
		// be subtracted from the number to get the value in integer form.
		col += this_char - '0';
	}
	else if (this_char == '/')
	{
		col = 0;
		row++;
	}
	else if (this_char == ' ')
	{
		return char_pos + 1;
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
			case 'k': write_bitset_to_square(&pieces::king,   row, col, is_white); break;
			case 'q': write_bitset_to_square(&pieces::queen,  row, col, is_white); break;
			case 'r': write_bitset_to_square(&pieces::rook,   row, col, is_white); break;
			case 'b': write_bitset_to_square(&pieces::bishop, row, col, is_white); break;
			case 'n': write_bitset_to_square(&pieces::knight, row, col, is_white); break;
			case 'p': write_bitset_to_square(&pieces::pawn,   row, col, is_white); break;
		}
		++col;
	}

	return read_fen_main(char_pos + 1, row, col);
}

char *ChessBoard::read_fen_castle(char *castle_str)
{
	switch (*castle_str)
	{
		default: break;
		case 'K': castle_rights[0] = true; break;
		case 'Q': castle_rights[1] = true; break;
		case 'k': castle_rights[2] = true; break;
		case 'q': castle_rights[3] = true; break;
		case '-': return castle_str + 2;
		case ' ': return castle_str + 1;
	}
	return read_fen_castle(castle_str + 1);
}

char *ChessBoard::read_fen_enp(char *enp_str)
{
	if (*enp_str == '-')
	{
		return enp_str + 2;
	}

	int col = tolower(*enp_str) - 'a';
	int row = 7 - (enp_str[1] - '1');

	write_bitset_to_square(&pieces::pawn_e, row, col, !white_turn);
	return enp_str + 3;
}

void ChessBoard::write_bitset_to_square(const std::bitset<3> *piece, int row, int col, bool is_white)
{
	for (int i = 0; i < 3; i++)
	{
		board_raw_w(row, col, i, (*piece)[i]);
	}
	board_raw_w(row, col, 3, is_white);
}

bool ChessBoard::board_raw_r(int row, int col, int bit)
{
	return board_raw[row*8*4+col*4+bit];
}

void ChessBoard::board_raw_w(int row, int col, int bit, bool val)
{
	board_raw[row*8*4+col*4+bit] = val;
}