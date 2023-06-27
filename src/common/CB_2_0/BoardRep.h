#ifndef CB_2_0_BR
#define CB_2_0_BR

#include <vector>
#include <stack>
#include <cstdint>
#include <string>
#include <bit>

#include "Move.h"
#include "Utils.h"
#include "tables/move_tables.h"
#include "tables/tf_table.h"

namespace CB
{

class BoardRep
{
public:
	BoardRep();
	BoardRep(const char fen[]);

	unsigned int get_board_state();

	void wipe_board();
	// Writes the given fen string to the board.
	// Improper format can cause undefined behavior.
	void write_fen(const char *fen);
	// Writes board info into an 8x8 character array.
	void board_to_str(char board[8][8]);

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
	// Takes a VERIFIED LEGAL to-from combination and formats it
	Move format_mv(unsigned int to, unsigned int from);

	~BoardRep();

private:
	std::stack<Move> move_history;
	std::stack<board_state_extra> state_history;

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
	uint64_t pawn_smear(bool is_white);
	// This function cannot be used to get king masks. It generates the
	// mask of moves that will block a direct check.
	void update_check_blocks();
	void update_pins();
	uint64_t xray_ratk(uint64_t occ, uint64_t blockers, int sq);
	uint64_t xray_batk(uint64_t occ, uint64_t blockers, int sq);

	// DOES NOT HANDLE SPECIAL MOVES
	uint64_t get_legal_mv_mask(int sq);
	uint64_t get_pseudo_mv_mask(int sq);

	void king_mv_adjust(int sq, uint64_t &moves);
	uint64_t get_pin_mask(int sq);

	// DOES NOT HANDLE SPECIAL MOVES
	uint64_t pawn_moves(int sq);

	// Special moves should be added to move masks with these functions
	uint64_t get_castle_mask();
	uint64_t get_enp_mask(int sq);

	// Use this to append all captures and pushes to the move_list
	void append_simple_moves(std::vector<Move> *move_list);
	// Use these to append special moves to the move list
	void append_castle_moves(std::vector<Move> *move_list);
	void append_enp_moves(std::vector<Move> *move_list);
	void append_promos(std::vector<Move> *move_list);

	bool ksc_legal();
	bool qsc_legal();

	void write_piece(bool is_white, int pid, int sq);
	void delete_piece(int sq);

	int get_pid(int sq);

};

}

#endif
