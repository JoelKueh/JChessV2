#ifndef CB_2_0_BR
#define CB_2_0_BR

#include <vector>
#include <cstdint>
#include <string>
#include <bit>

#include "Move.h"
#include "Utils.h"
#include "tables/move_tables.h"
#include "tables/tf_table.h"

/*
 * This file contains the class that describes a ChessBoard.
 * The representation is mostly based on the bitboard representation described
 * on chessprogramming.org, but also contains a redundant maibox representation
 * to quickly answer the question "Which piece is on square X?".
 *
 * AI users of this class need only initialize the board to a given
 * position, then call get_mv_set to generate the needed set of moves at a
 * given position. This will return a heap-allocated vector populated with all
 * legal moves at a given position.
 *
 * UI users of this class need only call get_mv_set to get a struct of four
 * bitboards at a given position that represents the legal moves that can
 * be made from a given square at a given position.
 *
 * When a move is made on the ChessBoard, many of the different tables will be
 * invalidated. Tables containing pin and check information are need to be
 * updated upon every iteration. This is hidden from the user so that tables
 * are only updated exactly when they are needed. E.G. gen_move_list() updates
 * all tables before generating any moves.
 *
 * The board representation follows the make/unmake philosophy rather than the
 * copy make philosophy. This is mostly because tables need not be regenerated
 * when revisiting a node. They only need to be generated when generating the
 * move set for a particular node, which only needs to be done once. See the
 * perft algorithm implemented in cli_debug for more information.
 */
namespace CB
{

class BoardRep
{
public:
	BoardRep();
	BoardRep(char fen[]);

	unsigned int get_board_state() const;

	void wipe_board();
	// Writes the given fen string to the board.
	// Improper format can cause undefined behavior.
	void write_fen(const char *const fen_str);
	// Writes board info into an 8x8 character array.
	void board_to_str(char board_str[8][8]) const;

	// Generates the list of legal moves at the given position.
	// Memory is allocated to the heap and should be freed when done.
	std::vector<Move> *gen_move_list();
	// Populates mask with three bitboards containing the legal push,
	// capture, and special moves at a given square
	void get_mv_set(move_set *mask, int sq);
	// Makes a move
	void make(const Move &move);
	// Undoes the last move in the history
	void unmake();
	// Takes a to-from combination and verifies it before returning a move
	Move format_mv(unsigned int to, unsigned int from, pid promo_piece);

	bool is_white_turn() { return white_turn; }

	~BoardRep() = default;

private:
	std::vector<Move> move_history;
	std::vector<board_state_extra> state_history;
	int full_move_number = 0;

	struct {
		uint64_t color[2];
		uint64_t piece[2][6];
		uint64_t occupied;
	} bb;
	int mailbox[64];
	bool white_turn;

	bool tables_valid = false;
	uint64_t threats;
	uint64_t checkers;
	uint64_t check_blocks;
	uint64_t pins[9];

	void update_all();
	void update_checks();
	// A helper for update_checks. Takes the pawn bitboard then shifts it
	// diagonally in both directions to get the set of squares attacked
	// by pawns
	uint64_t pawn_smear(bool is_white) const;
	// This function cannot be used to get king masks. It generates the
	// mask of moves that will block a direct check.
	void update_check_blocks();
	void update_pins();
	uint64_t xray_ratk(uint64_t occ, uint64_t blockers, int sq) const;
	uint64_t xray_batk(uint64_t occ, uint64_t blockers, int sq) const;

	// DOES NOT HANDLE SPECIAL MOVES
	uint64_t get_legal_mv_mask(int sq) const;
	uint64_t get_pseudo_mv_mask(int sq) const;

	void king_mv_adjust(int sq, uint64_t &moves) const;
	uint64_t get_pin_mask(int sq) const;

	// DOES NOT HANDLE SPECIAL MOVES
	uint64_t pawn_moves(int sq) const;

	// Special moves should be added to move masks with these functions
	uint64_t get_castle_mask() const;
	uint64_t get_pawn_sp_mask(int sq) const;

	// Takes a VERIFIED LEGAL to from combination for a quiet or capture
	// and formats it into a move. For use exclusively in append_simple_moves()
	Move format_simple_mv(unsigned int to, unsigned int from);
	Move format_promo_mv(unsigned int to, unsigned int from, pid promo_piece);
	// Use this to append all captures and pushes to the move_list
	void append_simple_moves(std::vector<Move> *move_list);
	// Use these to append special moves to the move list
	void append_castle_moves(std::vector<Move> *move_list);
	void append_enp_moves(std::vector<Move> *move_list);
	void append_dpawn_push(std::vector <Move> *move_list);
	void append_promos(std::vector<Move> *move_list);

	bool ksc_legal() const;
	bool qsc_legal() const;

	void write_piece(bool is_white, int pid, int sq);
	void delete_piece(int sq);

	void parse_fen_main(const char to_parse, int &sq);

	int get_pid(int sq) const;
	bool sq_is_white(int sq) const;
	char sq_to_char(int sq) const;

};

}

#endif
