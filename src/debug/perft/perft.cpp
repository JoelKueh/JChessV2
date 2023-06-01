#include "../../common/chessboard/BoardRep.h"
#include <iostream>
#include <cstdint>
#include <string>

ChessBoard::BoardRep board;

std::string parse_args(int argc, char **argv)
{
	std::string fen;
	for (int i = 1; i < argc; ++i) {
		fen.append(argv[i]);
		fen.push_back(' ');
	}

	return fen;
}

uint64_t perft(int depth)
{
	ChessBoard::BoardRep::move move_list[218];
	int n_moves, i;
	uint64_t nodes = 0;

	if (depth == 0) 
		return 1ULL;

	board.gen_move_list(move_list);
	for (i = 0; i < n_moves; i++) {
		board.make_mv(move_list[i]);
		nodes += perft(depth - 1);
		board.unmake_mv();
	}
	return nodes;
}

int main(int argc, char **argv)
{
	std::string fen = parse_args(argc, argv);
	board.fen_to_board(fen.c_str());

	uint64_t nodes = perft(6);

	std::cout << "Perft for fen string: " << fen << std::endl
		<< "completed successfully with " << nodes
		<< " nodes searched." << std::endl;
	
	return 0;
}
