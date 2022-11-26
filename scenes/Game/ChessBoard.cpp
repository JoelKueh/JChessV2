#include "ChessBoard.h"

// DEBUG
#include <iostream>
#include <ncurses.h>

ChessBoard::ChessBoard()
{
	for (int i = 0; i < board_raw.size(); i++)
	{
		board_raw[i] = 0;
	}
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
				piece[bit] = board_raw(row,col,bit);
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

			if (board_raw(row,col,0) == 1)
			{
				toupper(output[row][col]);
			}
		}
	}

	return output;
}