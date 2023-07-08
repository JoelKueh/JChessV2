#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include "../../common/CB_2_0/BoardRep.cpp"

void parse_command(CB::BoardRep board, const std::string &command, std::stringstream &args);
void parse_position(CB::BoardRep board, std::stringstream &args);
void parse_move(CB::BoardRep &board, std::stringstream &args);
void print_board(const CB::BoardRep &board);
CB::Move algbr_to_move(CB::BoardRep &board, std::string &algebraic);

const char np_line[] = "+---+---+---+---+---+---+---+---+";

int main(int argc, char *argv[])
{
	char *fen;
	char init_fen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	if (argc == 1) {
		fen = init_fen;
	} else if (argc == 2) {
		fen = argv[1];
	} else {
		std::cout << "Usage ./new_debug \"FEN\" or just ./new_debug" << std::endl;
		return -1;
	}

	CB::BoardRep my_board(fen);

	std::string input;
	std::string command;
	bool cont = true;
	while (cont) {
		std::getline(std::cin, input);
		std::stringstream parser(input);

		parser >> command;
		for (int i = 0; i < command.size(); ++i)
			command.at(i) = tolower(command.at(i));

		parse_command(my_board, command, parser);
	}
}

void parse_command(CB::BoardRep board, const std::string &command, std::stringstream &args)
{
	if (command == "position") {
		parse_position(board, args);
	} else if (command == "help") {
		std::cout << "Commands:\n"
			<< "Position:\n\t"
			<< "Args: fen <QUOTE_SURROUNDED_FEN_STRING> moves <LIST_OF_MOVES>"
			<< "Move:\n\t"
			<< "Args: A list of moves given by To-From combinations"
			<< "with a letter at the end for any promoted piece\n"
			<< "d: Displays the board" << std::endl;
	} else if (command == "move") {
		parse_move(board, args);
	} else if (command == "d") {
		print_board(board);
	} else if (command == "exit") {
		std::cout << "Quitting..." << std::endl;
		exit(0);
	} else {
		std::cout << "Invalid command, Use \"help\" to see a list of commands." << std::endl;
	}
}

void parse_position(CB::BoardRep board, std::stringstream &args)
{
	std::string fen;
	std::string next;

	args >> next;
	while (args && next != "move") {
		fen.push_back(' ');
		fen += next;

		args >> next;
		for (int i = 0; i < next.size(); ++i)
			next.at(i) = tolower(next.at(i));
	}

	std::vector<CB::Move> move_list;
	while (args) {
		args >> next;
		move_list.push_back(algbr_to_move(board, next));
		if (move_list.back().is_invalid()) {
			std::cout << "INVALID MOVE, SKIPPING MOVE EXECUTION";
			std::cout << std::endl;
			return;
		}
	}

	while (move_list.size() != 0) {
		board.make(move_list.back());
		move_list.pop_back();
	}
}

void parse_move(CB::BoardRep &board, std::stringstream &args)
{
	std::string algbr;
	args >> algbr;

	CB::Move move = algbr_to_move(board, algbr);
	if (move.is_invalid()) {
		std::cout << "INVALID MOVE" << std::endl;
		return;
	}

	board.make(move);
}

void print_board(const CB::BoardRep &board)
{
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

CB::Move algbr_to_move(CB::BoardRep &board, std::string &algebraic)
{
	for (int i = 0; i < algebraic.size(); ++i) {
		algebraic.at(i) = tolower(algebraic.at(i));
	}
	int to = (7 - (algebraic.at(0) - 'a')) * 8 + (algebraic.at(1) - '0');
	int from = (7 - (algebraic.at(2) - 'a')) * 8 + (algebraic.at(3) - '0');

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
