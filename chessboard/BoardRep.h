#pragma once

#include <cstdint>
#include <string>
#include "move_gen/move_tables.h"
#include "tf_table.h"

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

	BoardRep();
	bool white_turn;
	short halfmove_clock = 0;
	short fullmove_number = 0;
	char** board_to_strarr();
	char square_to_char(int square);
	char up_if_white(char piece, bool is_white);

	void wipe_board();
	void fen_to_board(char *fen_str);

	int get_board_state();
	uint64_t get_legal_moves(int sq);
	uint64_t get_pseudo_moves(int sq);
	int *get_moves_for_display(int sq);

private:

	inline uint64_t atk_pawn(bool is_white, int sq);
	inline uint64_t atk_wpawn(int sq);
	inline uint64_t atk_bpawn(int sq);
	inline uint64_t mv_pawn(bool is_white, int sq);
	inline uint64_t mv_wpawn(int sq);
	inline uint64_t mv_bpawn(int sq);
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

	// Updated with every move, 0 if false, 1 if single check, 2 if double
	// or greater.
	short king_in_check[2];
	// Stores all of the special moves that are currently possible.
	// First four bits represent represent the catling rights. The 5th bit
	// represents the availiabitlity of an enpassant. And the last three bits
	// specify the column of that enpassant.
	uint8_t special_moves;

	char *read_fen_main(char *start_char, int row = 7, int col = 0);
	char *read_fen_castle(char *castle_str);
	char *read_fen_enp(char *enp_str);

	void write_piece(char piece, int square);
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