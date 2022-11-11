#include "ChessBoard.h"

ChessBoard::ChessBoard() = default;

char** ChessBoard::read_board()
{
	char_row* output = new char_row[8];

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			std::bitset<4> piece;
			for (int bit = 0; bit < 4; bit++)
			{
				piece[bit] = board_raw(row,col,bit + 1);
			}

			switch (piece)
			{
				case none: output[row][col] = " "; break;
				case pawn_c: output[row][col] = "p"; break;
				case pawn: output[row][col] = "p"; break;
				case knight: output[row][col] = "k"; break;
				case bishop: output[row][col] = "b"; break;
				case rook_c: output[row][col] = "r"; break;
				case rook: output[row][col] = "r"; break;
				case queen: output[row][col] = "q"; break;
				case king: output[row][col] = "k"; break;
			}

			if (board_raw(row,col))
			{
				toupper(output[row][col]);
			}
		}
	}
}
