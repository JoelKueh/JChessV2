#pragma once

#include <cstdint>
#include <string>
#include <bit>
#include "move_gen/move_tables.h"
#include "tf_table.h"
#include "dir_table.h"

namespace ChessBoard {

class BoardRep
{
public:
	enum board_states
	{
		normal,
		white_in_check,
		black_in_check,
		white_win,
		black_win,
		stalemate,
	};

	struct coord {
		int row;
		int col;
	};

	struct move_mask {
		uint64_t push;
		uint64_t cap;
		uint64_t special;
	};

	BoardRep();
	bool white_turn;
	short halfmove_clock = 0;
	short fullmove_number = 0;
	void board_to_strarr(char brdstr[8][8]);
	char square_to_char(int square);
	char up_if_white(char piece, bool is_white);

	void wipe_board();
	void fen_to_board(char *fen_str);

	int get_board_state();
	int get_checks(bool is_white);
	uint64_t get_legal_moves(int sq);
	uint64_t get_pseudo_moves(int sq);
	void get_mv_mask(move_mask *mask, int sq);

	void update_pins_and_checks();
	void update_pins_and_checks(bool is_white);

	// DEBUG: THIS SHOULD BE MADE PRIVATE
	void write_piece(char piece, int square);
	void delete_piece(int sq);

private:

	inline void pin_adjust(int sq, uint64_t *moves);
	inline void pin_adjust(int sq, uint64_t *moves, bool is_white);
	inline void check_adjust(int sq, uint64_t *moves);
	inline void check_adjust(int sq, uint64_t *moves, bool is_white);
	inline bool seen_by_king(int sq);
	inline int find_king(bool is_white);

	inline uint64_t atk_pawn(bool is_white, int sq);
	inline uint64_t atk_wpawn(int sq);
	inline uint64_t atk_bpawn(int sq);
	inline uint64_t mv_pawn(bool is_white, int sq);
	inline uint64_t mv_wpawn(int sq);
	inline uint64_t mv_bpawn(int sq);

	// Not Used?
	inline uint64_t enp_pawn(bool is_white, int sq);
	inline uint64_t enp_wpawn(int sq);
	inline uint64_t enp_bpawn(int sq);

	inline uint64_t dmv_pawn(bool is_white, int sq);
	inline uint64_t dmv_wpawn(int sq);
	inline uint64_t dmv_bpawn(int sq);
	inline uint64_t atk_knight(int sq);
	inline uint64_t atk_bishop(int sq);
	inline uint64_t atk_rook(int sq);
	inline uint64_t atk_queen(int sq);
	inline uint64_t atk_king(int sq);

	uint64_t get_legal_castle_mask(int sq, bool is_white);
	bool can_ksk(bool is_white);
	bool can_qsk(bool is_white);
 
	uint64_t get_legal_enp_mask(int sq, bool is_white);

	bool is_threatened(int sq);
	bool is_threatened(int sq, bool is_white);
	
	const uint64_t WHITE_KSK = 0x4000000000000000;
	const uint64_t BLACK_KSK = 0x0000000000000040;
	const uint64_t WHITE_QSK = 0x0400000000000000;
	const uint64_t BLACK_QSK = 0x0000000000000004;

	// A bitset representation of the board. Each piece type has
	// its own bitset. There are also union bitsets: white, black,
	// and occupied.
	struct board_base {
		uint64_t color[2];

		uint64_t pawn[2];
		uint64_t knight[2];
		uint64_t bishop[2];
		uint64_t rook[2];
		uint64_t queen[2];
		uint64_t king[2];
	
		uint64_t occupied;
	}; board_base my_board;

	// The curent squares that are checking the king.
	uint64_t checkers[2];
	// Stores all of the special moves that are currently possible.
	// First four bits represent represent the catling rights. The 5th bit
	// represents the availiabitlity of an enpassant. And the last three bits
	// specify the column of that enpassant.
	uint8_t special_moves;
	// Stores all of the rays that contain pinned pieces. Slot 8
	// in each array stores the union of valid rays storid in slots 0-7.
	// Slots 0-7 are not zeroed out when this is updated. So the union must
	// be checked first to see if the piece is actually pinned, then the
	// actuall pinning ray can be found by searching from zero up.
	uint64_t pins[2][9];

	char *read_fen_main(char *start_char, int row = 0, int col = 0);
	char *read_fen_castle(char *castle_str);
	char *read_fen_enp(char *enp_str);

	inline void set_enp(int col);
	/**
	 * @brief Reads the special_moves byte for enp.
	 * 
	 * @return Column of the enp piece (-1 if none are availiable).
	 */
	inline int get_enp();

	void execute_move();
	void undo_move();
};

}
