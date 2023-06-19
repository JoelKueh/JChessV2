#include "../../common/chessboard/BoardRep.h"
#include <iostream>
#include <cstdint>
#include <string>
#include <chrono>
#include <unistd.h>

ChessBoard::BoardRep board;

std::string parse_args(int argc, char **argv)
{
	std::string fen;
	for (int i = 2; i < argc; ++i) {
		fen.append(argv[i]);
		fen.push_back(' ');
	}

	return fen;
}

std::string to_algebraic(int sq)
{
	std::string out;
	out.push_back('a' + sq % 8);
	out.push_back('8' - sq / 8);
	return out;
}

uint64_t perft(int depth)
{
	ChessBoard::BoardRep::move move_list[219];
	int i;
	uint64_t nodes = 0;

	if (depth == 0) 
		return 1ULL;

	board.gen_move_list(move_list);
	for (i = 0; move_list[i].valid; i++) {
		board.make_mv(move_list[i]);
		nodes += perft(depth - 1);
		board.unmake_mv();
	}
	return nodes;
}

uint64_t perft_top(int depth)
{
	ChessBoard::BoardRep::move move_list[219];
	int i;
	uint64_t nodes = 0;

	board.gen_move_list(move_list);
	for (i = 0; move_list[i].valid; i++) {
		board.make_mv(move_list[i]);
		uint64_t result = perft(depth - 1);
		nodes += result;
		board.unmake_mv();

		std::cout << to_algebraic(move_list[i].from)
			<< to_algebraic(move_list[i].to)
			<< ": " << result << std::endl;
	}
	return nodes;
}

int main(int argc, char **argv)
{
	std::string fen = parse_args(argc, argv);
	board.fen_to_board(fen.c_str());

	auto start = std::chrono::high_resolution_clock::now();

	uint64_t nodes = perft_top(atoi(argv[1]));

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Perft for fen string: " << fen << std::endl
		<< "completed successfully with " << nodes
		<< " nodes searched " << std::endl
		<< "after " << duration.count() << " milliseconds." << std::endl;
	
	return 0;
}
