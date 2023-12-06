
#include <iostream>
#include "game_utils.h"

CB::Move algbr_to_move(CB::BoardRep &board, std::string &algebraic)
{
	for (int i = 0; i < algebraic.size(); ++i) {
		algebraic.at(i) = tolower(algebraic.at(i));
	}

	int to = (algebraic.at(2) - 'a') + (7 - (algebraic.at(3) - '1')) * 8;
	int from = (algebraic.at(0) - 'a') + (7 - (algebraic.at(1) - '1')) * 8;

	if (algebraic.size() == 4)
		return board.format_mv(to, from, CB::EMPTY);

	CB::pid promo_pid;
	switch (algebraic.at(4)) {
		case 'n': promo_pid = CB::KNIGHT; break;
		case 'b': promo_pid = CB::BISHOP; break;
		case 'r': promo_pid = CB::ROOK; break;
		case 'q': promo_pid = CB::QUEEN; break;
		default: return CB::Move(CB::Move::INVALID);
	}
	return board.format_mv(to, from, promo_pid);
}

void print_board(const CB::BoardRep &board)
{
	const char np_line[] = "+---+---+---+---+---+---+---+---+";
	char board_str[8][8];
	board.board_to_str(board_str);

	for (int row = 0; row < 8; ++row) {
		std::cout << np_line << std::endl << "| ";
		for (int col = 0; col < 8; ++col) {
			std::cout << board_str[row][col] << " | ";
		}
		std::cout << std::endl;
	}
	std::cout << np_line << std::endl;
}
