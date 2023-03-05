#include "BoardRep.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0') << \
  (byte & 0x40 ? '1' : '0') << \
  (byte & 0x20 ? '1' : '0') << \
  (byte & 0x10 ? '1' : '0') << \
  (byte & 0x08 ? '1' : '0') << \
  (byte & 0x04 ? '1' : '0') << \
  (byte & 0x02 ? '1' : '0') << \
  (byte & 0x01 ? '1' : '0') 


#define U64_TO_BB(file, u64)  \
  file << BYTE_TO_BINARY(u64 >> 56) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 48) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 40) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 32) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 24) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 16) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 8) << std::endl \
    << BYTE_TO_BINARY(u64) << std::endl << std::endl; \
/////////////////////////////

ChessBoard::BoardRep::BoardRep()
{
	MoveTables::gen_move_tables();
	char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	fen_to_board(init_board);
}

int *ChessBoard::BoardRep::get_moves_for_display(int sq)
{
	int output[64];
	
}

uint64_t ChessBoard::BoardRep::get_pseudo_moves(int sq)
{
	uint64_t mask = 1ULL << sq;
	bool color = mask & my_board.color[1];
	if (mask & (~my_board.occupied))
		return 0;

	if (mask & my_board.pawn[color]) {
		uint64_t output = 0;
		output |= atk_pawn(color, sq);
		output |= mv_pawn(color, sq);
		return output;
	}

	if (mask & my_board.knight[color])
		return atk_knight(sq);

	if (mask & my_board.bishop[color])
		return atk_bishop(sq);

	if (mask & my_board.rook[color])
		return atk_rook(sq);

	if (mask & my_board.queen[color])
		return atk_queen(sq);

	if (mask & my_board.king[color])
		return atk_king(sq);

	return 0;
}

// Generates all of the attacks that a pawn can make on a certain square.
// Does not handle enpassant.
inline uint64_t ChessBoard::BoardRep::atk_pawn(bool color, int sq)
{	
	if (color)
		return atk_wpawn(sq);
	else
		return atk_bpawn(sq);
}

inline uint64_t ChessBoard::BoardRep::atk_wpawn(int sq)
{
	uint64_t output = 0;
	uint64_t not_rcol = 0xFEFEFEFEFEFEFEFEULL;
	uint64_t not_lcol = 0x7F7F7F7F7F7F7F7FULL;

	// This may overflow, but that should not matter as overflow of a r-shift
	// becomes zero.
	output |= (1ULL << sq + 9) & not_rcol;
	output |= (1ULL << sq + 7) & not_lcol;

	return output;
}

inline uint64_t ChessBoard::BoardRep::atk_bpawn(int sq)
{
	uint64_t output = 0;
	uint64_t not_rcol = 0xFEFEFEFEFEFEFEFEULL;
	uint64_t not_lcol = 0x7F7F7F7F7F7F7F7FULL;

	// This may overflow, but that should not matter as overflow of a r-shift
	// becomes zero.
	output |= (1ULL << sq - 9) & not_lcol;
	output |= (1ULL << sq - 7) & not_rcol;

	return output;
}

// Generates the non-threatening moves that a pawn can make.
// Does not handle double moves.
inline uint64_t ChessBoard::BoardRep::mv_pawn(bool color, int sq)
{
	if (color)
		return mv_wpawn(sq);
	else
		return mv_bpawn(sq);
}

inline uint64_t ChessBoard::BoardRep::mv_wpawn(int sq)
{
	return (1ULL << sq + 8) & (~my_board.occupied);
}

inline uint64_t ChessBoard::BoardRep::mv_bpawn(int sq)
{
	return (1ULL << sq - 8) & (~my_board.occupied);
}

inline uint64_t ChessBoard::BoardRep::dmv_pawn(bool color, int sq)
{
	if (color)
		return dmv_wpawn(sq);
	else
		return dmv_bpawn(sq);
}

inline uint64_t ChessBoard::BoardRep::dmv_wpawn(int sq)
{
	if (sq >= 16 || sq < 8)
		return 0;
	
	uint64_t mask = (1ULL << sq + 8) | (1ULL << sq + 16);
	if (mask & (~my_board.occupied))
		return (1ULL << sq + 16);

	return 0;
}

inline uint64_t ChessBoard::BoardRep::dmv_bpawn(int sq)
{
	if (sq >= 56 || sq < 48)
		return 0;
	
	uint64_t mask = (1ULL << sq - 8) | (1ULL << sq - 16);
	if (mask & (~my_board.occupied))
		return (1ULL << sq - 16);

	return 0;
}

inline uint64_t ChessBoard::BoardRep::enp_pawn(bool color, int sq)
{
	if (color)
		return enp_wpawn(sq);
	else
		return enp_bpawn(sq);
}

inline uint64_t ChessBoard::BoardRep::enp_wpawn(int sq)
{
	int enp_col = get_enp();
	if (enp_col == -1)
		return 0;

	if (sq < 24 || sq >= 32)
		return 0;

	if (sq % 8 + 1 == enp_col)
		return 1ULL << sq + 9;
	else if (sq % 8 - 1 == enp_col)
		return 1ULL << sq + 7;

	return 0;
}

inline uint64_t ChessBoard::BoardRep::enp_bpawn(int sq)
{
	int enp_col = get_enp();
	if (enp_col == -1)
		return 0;

	if (sq < 32 || sq >= 40)
		return 0;

	if (sq % 8 + 1 == enp_col)
		return 1ULL << sq - 7;
	else if (sq % 8 - 1 == enp_col)
		return 1ULL << sq - 9;

	return 0;
}

inline uint64_t ChessBoard::BoardRep::atk_knight(int sq)
{
	return MoveTables::read_natk(sq);
}

inline uint64_t ChessBoard::BoardRep::atk_bishop(int sq)
{
	return MoveTables::read_batk(sq, my_board.occupied);
}

inline uint64_t ChessBoard::BoardRep::atk_rook(int sq)
{
	return MoveTables::read_ratk(sq, my_board.occupied);
}

inline uint64_t ChessBoard::BoardRep::atk_queen(int sq)
{
	uint64_t output = MoveTables::read_batk(sq, my_board.occupied);
	output |= MoveTables::read_ratk(sq, my_board.occupied);
	return output;
}

inline uint64_t ChessBoard::BoardRep::atk_king(int sq)
{
	return MoveTables::read_katk(sq);
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

	special_moves = 0;
}

char *ChessBoard::BoardRep::read_fen_main(char *char_pos, int row, int col)
{
	char this_char = *char_pos;
	if (isdigit(this_char))
	{
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

inline int ChessBoard::BoardRep::get_enp()
{
	if (0b00010000 & (~special_moves))
		return -1;
	
	return special_moves >> 5;
}