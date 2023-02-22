#include "BoardRep.h"

ChessBoard::BoardRep::BoardRep()
{
	char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	fen_to_board(init_board);
}

char** ChessBoard::BoardRep::board_to_strarr()
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
			output[row][col] = square_to_char(row * 8 + col);
		}
	}
	return output;
}

char ChessBoard::BoardRep::square_to_char(int square)
{
	uint64_t manip = UINT64_C(1) << square;

	if (my_board.occupied & manip == 0)
		return ' ';

	// If there is a white piece on square, then is_white is true, else it is false.
	bool is_white = my_board.color[1] & manip;

	if (my_board.pawn[is_white] & manip)
		return up_if_white('p', is_white);

	if (my_board.knight[is_white] & manip)
		return up_if_white('n', is_white);

	if (my_board.bishop[is_white] & manip)
		return up_if_white('b', is_white);

	if (my_board.rook[is_white] & manip)
		return up_if_white('r', is_white);
	
	if (my_board.queen[is_white] & manip)
		return up_if_white('q', is_white);

	if (my_board.king[is_white] & manip)
		return up_if_white('k', is_white);

	return ' ';
}

char ChessBoard::BoardRep::up_if_white(char piece, bool is_white)
{
	if (is_white)
		return toupper(piece);
	return piece;
}

void ChessBoard::BoardRep::fen_to_board(char *fen_str)
{
	wipe_board();

	char *char_pos;
	char_pos = read_fen_main(fen_str);
	
	white_turn = false;
	if (*char_pos == 'w')
	{
		white_turn = true;
	}
	char_pos += 2;

	char_pos = read_fen_castle(char_pos);
	char_pos = read_fen_enp(char_pos);

	halfmove_clock = *char_pos - '0';
	char_pos += 2;
	fullmove_number = *char_pos - '0';
}

void ChessBoard::BoardRep::wipe_board()
{
	my_board.color[0] = 0;
	my_board.pawn[0] = 0;
	my_board.knight[0] = 0;
	my_board.bishop[0] = 0;
	my_board.rook[0] = 0;
	my_board.queen[0] = 0;
	my_board.king[0] = 0;

	my_board.color[1] = 0;
	my_board.pawn[1] = 0;
	my_board.knight[1] = 0;
	my_board.bishop[1] = 0;
	my_board.rook[1] = 0;
	my_board.queen[1] = 0;
	my_board.king[1] = 0;

	my_board.occupied = 0;

	for (int i = 0; i < 64; i++) {
		attacks_from[i] = 0;
		attacks_to[i] = 0;
	}

	special_moves = 0;
}

char *ChessBoard::BoardRep::read_fen_main(char *char_pos, int row, int col)
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
		row--;
	}
	else if (this_char == ' ')
	{
		return char_pos + 1;
	}
	else
	{
		write_piece(this_char, row * 8 + col);
		++col;
	}

	return read_fen_main(char_pos + 1, row, col);
}

char *ChessBoard::BoardRep::read_fen_castle(char *castle_str)
{
	switch (*castle_str)
	{
	default: break;
	case 'K': special_moves |= 1;
	case 'Q': special_moves |= 1 << 1; break;
	case 'k': special_moves |= 1 << 2; break;
	case 'q': special_moves |= 1 << 3; break;
	case '-': return castle_str + 2;
	case ' ': return castle_str + 1;
	}
	return read_fen_castle(castle_str + 1);
}

char *ChessBoard::BoardRep::read_fen_enp(char *enp_str)
{
	if (*enp_str == '-')
	{
		return enp_str + 2;
	}

	int col = tolower(*enp_str) - 'a';
	
	set_enp(col);
	return enp_str + 3;
}

int ChessBoard::BoardRep::get_board_state()
{
	// Search for Check
	// Search for mate
	// Else search for stalemate
	// Return check, mate, or stalemate
	
	return board_states::normal;
}

void ChessBoard::BoardRep::write_piece(char piece, int square)
{
	uint64_t *piece_board;
	uint64_t *color_board;

	bool is_white = false;
	if (piece <= 'Z') {
		piece += 'a' - 'A';
		is_white = true;
	}

	switch (piece)
	{
	case 'p':
		piece_board = &my_board.pawn[is_white];
		color_board = &my_board.color[is_white];
		break;
	case 'n':
		piece_board = &my_board.knight[is_white];
		color_board = &my_board.color[is_white];
		break;
	case 'b':
		piece_board = &my_board.bishop[is_white];
		color_board = &my_board.color[is_white];
		break;
	case 'r':
		piece_board = &my_board.rook[is_white];
		color_board = &my_board.color[is_white];
		break;
	case 'q':
		piece_board = &my_board.queen[is_white];
		color_board = &my_board.color[is_white];
		break;
	case 'k': 
		piece_board = &my_board.king[is_white];
		color_board = &my_board.color[is_white];
		break;
	default:
		piece_board = &my_board.pawn[is_white];
		color_board = &my_board.color[is_white];
		break;
	}

	(*piece_board) |= UINT64_C(1) << square;
	(*color_board) |= UINT64_C(1) << square;
	my_board.occupied |= UINT64_C(1) << square;
}

inline void ChessBoard::BoardRep::set_enp(int col)
{
	special_moves = (special_moves & 0b11110000) ^ col << 5;
	special_moves = special_moves ^ (1 << 4);
}