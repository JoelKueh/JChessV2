#pragma once
#include <bitset>
#include "../../Global.h"

class ChessBoard
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

	ChessBoard();
	bool white_turn;
	short halfmove_clock = 0;
	short fullmove_number = 0;
	char** board_to_strarr();

	void write_strarr_to_board(char** board);
	void write_str_to_row(char* row);
	void write_str_to_col(char* col);
	void write_ch_to_square(char piece);

	void wipe_board();
	void fen_to_board(char *fen_str);

	int get_board_state();

	typedef char (char_row)[8];

	/**
	 * Returns the legal moves that a piece
	 * can make in a pointer to an std::bitset.
	 * Bitset contains 66 bits, 64 for the normal moves
	 * and 2 more for king and queen side castle, as
	 * they need to be handled differently.
	*/
	std::bitset<66> *get_moves(int row, int col);
private:
	/**
	 * A raw bitset version of the board, 3 bits are dedicated
	 * to each piece, and 1 bit represents the 
	*/
	std::bitset<256> board_raw;
	std::bitset<64> danger_black;
	std::bitset<64> danger_white;
	std::bitset<4> castle_rights {"1111"};

	bool board_raw_r(int row, int col, int bit);
	void board_raw_w(int row, int col, int bit, bool val);

	char *read_fen_main(char *start_char, int row = 0, int col = 0);
	char *read_fen_castle(char *castle_str);
	char *read_fen_enp(char *enp_str);

	void write_bitset_to_square(const std::bitset<3> *piece, int row, int col, bool is_white);
	
	/**
	 * Returns true if the piece at (row, col) matches the std::bitset
	 * at *piece.
	 * @param piece A pointer to the global constant piece for comparison.
	 * @param row The row of the square to be checked.
	 * @param col The column of the square to be checked.
	*/
	bool is_piece(std::bitset<3> *piece, int row, int col);
	/**
	 * Gets the row and column of the defending_color's king.
	 * @return A coord struct representing the row and col of the king.
	*/
	coord *find_king(int king_color);
	/**
	 * Checks to see if the defending_color's king is in check
	 * @return 0 for no check, 1 for check, 2 for double check or more.
	 * @param defending_color The color of the defending king. 
	*/
	int in_check(int defending_color);
	/**
	 * Returns non-check-adjusted moves that a pawn could make
	 * at (row, col) as an std::bitset.
	*/
	std::bitset<66> *pawn_moves(int row, int col, short color);
	/**
	 * Returns non-check-adjusted moves that a knight could make
	 * at (row, col) as an std::bitset.
	 * @param row The piece's row.
	 * @param col The piece's column.
	 * @param color The color of the knight at the square.
	 */
	std::bitset<66> *knight_moves(int row, int col, short color);
	/**
	 * Returns non-check-adjusted moves that a bishop could make
	 * at (row, col) as an std::bitset.
	*/
	std::bitset<66> *bishop_moves(int row, int col, short color);
	/**
	 * Returns non-check-adjusted moves that a rook could make
	 * at (row, col) as an std::bitset.
	*/
	std::bitset<66> *rook_moves(int row, int col, short color);
	/**
	 * Returns non-check-adjusted moves that a rook could make
	 * at (row, col) as an std::bitset.
	*/
	std::bitset<66> *queen_moves(int row, int col, short color);
	/**
	 * Returns non-check-adjusted moves that a king could make
	 * at (row, col) as an std::bitset.
	*/
	std::bitset<66> *king_moves(int row, int col, short color);


	void execute_move();
	void undo_move();
};