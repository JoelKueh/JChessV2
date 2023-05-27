#include "BoardRep.h"
#include "move_gen/magics.h"
#include "tf_table.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

#define BYTE_TO_BINARY(byte)  \
  (byte & 0x01 ? '1' : '0') << \
  (byte & 0x02 ? '1' : '0') << \
  (byte & 0x04 ? '1' : '0') << \
  (byte & 0x08 ? '1' : '0') << \
  (byte & 0x10 ? '1' : '0') << \
  (byte & 0x20 ? '1' : '0') << \
  (byte & 0x40 ? '1' : '0') << \
  (byte & 0x80 ? '1' : '0')


#define U64_TO_BB(file, u64)  \
  file << BYTE_TO_BINARY(u64) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 8) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 16) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 24) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 32) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 40) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 48) << std::endl \
    << BYTE_TO_BINARY(u64 >> 56) << std::endl << std::endl \
/////////////////////////////

ChessBoard::BoardRep::BoardRep()
{
	MoveTables::gen_move_tables();
	
	// PROBABLY NOT NEEDED
	// TODO: REMOVE?
	// Fields::gen_dir_table();

	//char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	char init_board[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq f6 0 1";
	// char init_board[] = "2k5/8/8/8/3Q4/8/8/4K3 w KQkq - 0 1";
	fen_to_board(init_board);
	update_pins_and_checks();
}

int ChessBoard::BoardRep::get_checks(bool is_white)
{
	return std::__popcount(checkers[is_white]);
}

void ChessBoard::BoardRep::check_adjust(int sq, uint64_t *moves)
{
	bool is_white = (1ULL << sq) & my_board.color[1];
	check_adjust(sq, moves, is_white);
}

void ChessBoard::BoardRep::check_adjust(int sq, uint64_t *moves, bool is_white)
{
	uint64_t curr_sq = 1ULL << sq;
	if (my_board.king[is_white] & curr_sq)
		return king_mv_adjust(sq, moves, is_white);

	int num_checkers = std::__popcount(checkers[is_white]);

	if (num_checkers == 0)
		return;

	if (num_checkers >= 2 &&
		!(my_board.king[is_white] & curr_sq))
	{
		*moves = 0;
		return;
	}

	int king_sq = find_king(is_white);
	int checkr_sq = std::__countr_zero(checkers[is_white]);
	uint64_t blocks = Fields::tf_table[checkr_sq][king_sq];

	debug_out << "TF_TABLE" << std::endl;
	U64_TO_BB(debug_out, Fields::tf_table[sq][king_sq]);

	// DEBUG
	debug_out << "BLOCKS" << std::endl;
	U64_TO_BB(debug_out, blocks);
	debug_out << std::endl << std::endl;

	*moves &= blocks;
}

// TODO: MAKE THIS MORE EFFICIENT
// I cannot think of a better way to do this at the moment, other than
// an incrementally updated attack table.
//
// This also may be relavent, but I can't make heads or tails of it
// https://www.chessprogramming.org/Kogge-Stone_Algorithm
//
// PS: Yeah... This is probably really slow...
void ChessBoard::BoardRep::king_mv_adjust(int sq, uint64_t *moves, bool is_white)
{
	uint64_t to_parse = *moves;
	uint64_t board = my_board.occupied & ~(1ULL << find_king(is_white));

	while (to_parse)
	{
		int sq = std::__countr_zero(to_parse);
		to_parse ^= 1ULL << sq;

		uint64_t ratkrs = MoveTables::read_ratk(sq, board);
		ratkrs &= my_board.queen[!is_white] | my_board.rook[!is_white];
		uint64_t batkrs = MoveTables::read_batk(sq, board);
		batkrs &= my_board.queen[!is_white] | my_board.bishop[!is_white];
		uint64_t natkrs = MoveTables::read_natk(sq);
		natkrs &= my_board.knight[!is_white];

		if (ratkrs | batkrs | natkrs)
			*moves ^= 1ULL << sq;
	}
		
}

uint64_t ChessBoard::BoardRep::get_legal_enp_mask(int sq, bool is_white)
{
	uint64_t mask = enp_pawn(is_white, sq);
	if (!mask)
		return 0;

	uint64_t board = my_board.occupied;
	int row = is_white ? 4 : 3;
	uint64_t manip = (1ULL << sq)
		| (1ULL << (row * 8 + (special_moves & 0b11100000)));
	board &= ~manip;

	int enp_row = is_white ? 5 : 2;
	board |= 1ULL << (enp_row * 8 + (special_moves & 0b11100000));

	uint64_t ratkrs = board & MoveTables::read_ratk(sq, board);
	ratkrs &= my_board.rook[is_white] | my_board.queen[is_white];
	uint64_t batkrs = board & MoveTables::read_batk(sq, board);
	batkrs &= my_board.bishop[is_white] | my_board.queen[is_white];

	if (ratkrs | batkrs)
		return 0;

	return mask;
}

void ChessBoard::BoardRep::get_mv_mask(move_mask *mask, int sq)
{
	uint64_t manip = 1ULL << sq;
	bool is_white = manip & my_board.color[1];

	// Pseudo Moves contains both push and cap masks, the push mask
	// will be manipulated and then AND'ed with the color boards to
	// create thecap mask.
	mask->push = get_pseudo_moves(sq);
	pin_adjust(sq, &(mask->push));
	check_adjust(sq, &(mask->push));

	mask->cap = mask->push & my_board.color[!is_white];
	mask->push ^= mask->cap;
	mask->push &= ~my_board.color[is_white];
	
	// Handling special moves separately because they function differently
	// as far as pins and checks are concerned.
	//
	// They also have to be made and unmade differently, so it's best
	// to have them in their own section to save some computational power
	// later.
	mask->special = get_legal_castle_mask(sq, is_white);
	
	// Handle extra moves for pawn
	if (manip & my_board.pawn[is_white])
	{
		mask->cap = atk_pawn(is_white, sq) & my_board.color[!is_white];

		// This is a bit hacky, but should make double pawn
		// moves fairly fast.
		// It handles pins by assuming that the double move will be
		// blocked if and only if the single move was blocked.
		int dir = is_white ? -1 : 1;
		int row = is_white ? 6 : 1;
		if (!(1ULL << (sq + dir * 16) & my_board.occupied)
				&& (1ULL << (sq + dir * 8) & mask->push)
				&& (sq / 8 == row)) 
			mask->special = 1ULL << (sq + dir * 16);

		mask->special |= get_legal_enp_mask(sq, is_white);
	}

	// DEBUG
	debug_out << special_moves << std::endl;

	// DEBUG?
	update_pins_and_checks();
}

inline void ChessBoard::BoardRep::update_pins_and_checks()
{
	update_pins_and_checks(true);
	update_pins_and_checks(false);
}

inline void ChessBoard::BoardRep::update_pins_and_checks(bool is_white)
{
	// Zeroing old data (checkers doesn't need to be zero'd)
	pins[is_white][8] = 0;

	// Preliminary work
	int king_sq = find_king(is_white);
	uint64_t board = my_board.occupied;
	uint64_t checkers;
	uint64_t pinned = MoveTables::read_ratk(king_sq, board);
	pinned |= MoveTables::read_batk(king_sq, board);
	checkers = pinned & my_board.color[!is_white];

	// Handle Checks
	uint64_t rchkrs = MoveTables::read_ratk(king_sq, board);
	rchkrs &= (my_board.queen[!is_white] ^ my_board.rook[!is_white]);
	uint64_t bchkrs = MoveTables::read_batk(king_sq, board);
	bchkrs &= (my_board.queen[!is_white] ^ my_board.bishop[!is_white]);
	uint64_t nchkrs = MoveTables::read_natk(king_sq);
	nchkrs &= my_board.knight[!is_white];

	this->checkers[is_white] = rchkrs | bchkrs | nchkrs;

	// Hanlde Pins
	board ^= pinned & my_board.color[is_white];
	uint64_t ratkrs = MoveTables::read_ratk(king_sq, board);
	ratkrs &= (my_board.queen[!is_white] ^ my_board.rook[!is_white]);
	uint64_t batkrs = MoveTables::read_batk(king_sq, board);
	batkrs &= (my_board.queen[!is_white] ^ my_board.bishop[!is_white]);
	
	ratkrs &= ~pinned;
	batkrs &= ~pinned;

	int i = 0;
	while(ratkrs) {
		int sq = std::__countr_zero(ratkrs);
		ratkrs ^= 1ULL << sq;
		pins[is_white][i] = ChessBoard::Fields::tf_table[sq][king_sq];
		pins[is_white][8] ^= pins[is_white][i];

		++i;
	}

	while(batkrs) {
		int sq = std::__countr_zero(batkrs);
		batkrs ^= 1ULL << sq;
		pins[is_white][i] = ChessBoard::Fields::tf_table[sq][king_sq];
		pins[is_white][8] ^= pins[is_white][i];

		++i;
	}
}

inline void ChessBoard::BoardRep::pin_adjust(int sq, uint64_t *moves)
{
	bool is_white = (1ULL << sq) & my_board.color[1];
	pin_adjust(sq, moves, is_white);
}

inline void ChessBoard::BoardRep::pin_adjust(int sq, uint64_t *moves,
	bool is_white)
{
	uint64_t mask = 1ULL << sq;
	if (!(mask & pins[is_white][8]))
		return;

	uint64_t *rays = pins[is_white];
	// Iterate through pins until we find the ray that is pinning the piece.
	while (!(*(rays++) & mask));
	rays--;

	*moves &= *rays;
}

inline bool ChessBoard::BoardRep::seen_by_king(int sq)
{
	// TODO: TEST THIS, been way too long since I ever wrote it. Not sure
	// what it was supposed to do anymore
	uint64_t mask = atk_queen(find_king((1ULL << sq) & my_board.color[1]));
	return mask & (1ULL << sq);
}

inline int ChessBoard::BoardRep::find_king(bool is_white)
{
	return std::__countr_zero(my_board.king[is_white]);
}

uint64_t ChessBoard::BoardRep::get_pseudo_moves(int sq)
{
	uint64_t mask = 1ULL << sq;
	bool color = mask & my_board.color[1];
	if (mask & (~my_board.occupied))
		return 0;

	if (mask & my_board.pawn[color])
		return mv_pawn(color, sq);

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
	output |= (1ULL << (sq - 9)) & not_lcol;
	output |= (1ULL << (sq - 7)) & not_rcol;

	return output;
}

inline uint64_t ChessBoard::BoardRep::atk_bpawn(int sq)
{
	uint64_t output = 0;
	uint64_t not_rcol = 0xFEFEFEFEFEFEFEFEULL;
	uint64_t not_lcol = 0x7F7F7F7F7F7F7F7FULL;

	// This may overflow, but that should not matter as overflow of a r-shift
	// becomes zero.
	output |= (1ULL << (sq + 9)) & not_rcol;
	output |= (1ULL << (sq + 7)) & not_lcol;

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
	return (1ULL << (sq - 8)) & (~my_board.occupied);
}

inline uint64_t ChessBoard::BoardRep::mv_bpawn(int sq)
{
	return (1ULL << (sq + 8)) & (~my_board.occupied);
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
	if (sq >= 56 || sq < 48)
		return 0;
	
	uint64_t mask = (1ULL << (sq - 8)) | (1ULL << (sq - 16));
	if (mask & (~my_board.occupied))
		return (1ULL << (sq - 16));

	return 0;
}

inline uint64_t ChessBoard::BoardRep::dmv_bpawn(int sq)
{
	if (sq >= 16 || sq < 8)
		return 0;
	
	uint64_t mask = (1ULL << (sq + 8)) | (1ULL << (sq + 16));
	if (mask & (~my_board.occupied))
		return (1ULL << (sq + 16));

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
		return 1ULL << (sq - 7);
	else if (sq % 8 - 1 == enp_col)
		return 1ULL << (sq - 9);

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
		return 1ULL << (sq + 9);
	else if (sq % 8 - 1 == enp_col)
		return 1ULL << (sq + 7);

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

uint64_t ChessBoard::BoardRep::get_legal_castle_mask(int sq, bool is_white)
{
	uint64_t output = 0;
	debug_out << (uint32_t)special_moves << ' ' << sq << std::endl;

	// If we are on the king square AND have either of our casling rights
	// Then we proceed to check for castles
	if (!(sq == (is_white ? 60 : 4)
		&& special_moves & (3 << (0 + (is_white ? 0 : 2)))))
	{
		debug_out << "Failed Initial Check\n";
		return output;
	}

	// King side check
	output |= can_ksk(is_white) * (is_white ? WHITE_KSK : BLACK_KSK);
	debug_out << "CAN KSK\n";
	
	// Queen side check
	output |= can_qsk(is_white) * (is_white ? WHITE_QSK : BLACK_QSK);

	return output;
}

bool ChessBoard::BoardRep::can_ksk(bool is_white)
{
	int king_sq = is_white ? 60 : 4;

	// Check rights
	if (!(special_moves & (1 << (0 + (is_white ? 0 : 2)))))
		return false;

	// Check for moving through check.
	for (int i = 0; i < 3; ++i)
	{
		if (is_threatened(king_sq + i))
			return false;
	}

	if (Fields::tf_table[king_sq + 1][king_sq + 2] & my_board.occupied)
		return false;
	
	return true;
}

bool ChessBoard::BoardRep::can_qsk(bool is_white)
{
	int king_sq = is_white ? 60 : 4;

	// Check rights
	if (!(special_moves & (2 << (0 + (is_white ? 0 : 2)))))
		return false;

	// Check for moving through check.
	for (int i = 0; i < 3; ++i)
	{
		if (is_threatened(king_sq - i))
			return false;
	}

	if (Fields::tf_table[king_sq - 1][king_sq - 3] & my_board.occupied)
		return false;
	
	return true;
}

bool ChessBoard::BoardRep::is_threatened(int sq)
{
	bool is_white = 1ULL << sq & my_board.color[1];
	return is_threatened(sq, is_white);
}

bool ChessBoard::BoardRep::is_threatened(int sq, bool is_white)
{
	if (my_board.knight[!is_white] & atk_knight(sq))
		return true;

	if ((my_board.rook[!is_white] | my_board.queen[!is_white])
			& atk_rook(sq))
		return true;

	if ((my_board.bishop[!is_white] | my_board.queen[!is_white])
			& atk_bishop(sq))
		return true;

	if (my_board.pawn[!is_white] & atk_pawn(is_white, sq))
		return true;

	return false;
}

// IMPROPPER FREEING OF THIS IS DONE LITERALLY EVERYWHERE
// FIX ASAP
void ChessBoard::BoardRep::board_to_strarr(char brdstr[8][8])
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			brdstr[row][col] = square_to_char(row * 8 + col);
		}
	}
}

char ChessBoard::BoardRep::square_to_char(int square)
{
	uint64_t manip = UINT64_C(1) << square;

	if ((my_board.occupied & manip) == 0)
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
		++row;
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
	case ' ':

	default:
		piece_board = &my_board.pawn[is_white];
		color_board = &my_board.color[is_white];
		break;
	}

	(*piece_board) |= UINT64_C(1) << square;
	(*color_board) |= UINT64_C(1) << square;
	my_board.occupied |= UINT64_C(1) << square;
}

void ChessBoard::BoardRep::delete_piece(int sq)
{
	for (int i = 0; i <= 1; ++i) {
		my_board.pawn[i] &= ~(1ULL << sq);
		my_board.knight[i] &= ~(1ULL << sq);
		my_board.bishop[i] &= ~(1ULL << sq);
		my_board.rook[i] &= ~(1ULL << sq);
		my_board.queen[i] &= ~(1ULL << sq);
		my_board.king[i] &= ~(1ULL << sq);
		my_board.color[i] &= ~(1ULL << sq);
	}
	my_board.occupied &= ~(1ULL << sq);
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
