#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include "../../common/CB_2_0/BoardRep.cpp"

void parse_command(const std::string &command, std::stringstream &args);
void parse_position(std::stringstream &args);
void parse_move(std::stringstream &args);
void print_board();
CB::Move algbr_to_move(std::string algebraic);

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

		parse_command(command, parser);
	}
}

void parse_command(const std::string &command, std::stringstream &args)
{
	if (command == "position") {
		parse_position(args);
	} else if (command == "help") {
		std::cout << "Commands:\n"
			<< "Position:\n\t"
			<< "Args: fen <QUOTE_SURROUNDED_FEN_STRING> moves <LIST_OF_MOVES>"
			<< "Move:\n\t"
			<< "Args: A list of moves given by To-From combinations"
			<< "with a letter at the end for any promoted piece\n"
			<< "d: Displays the board" << std::endl;
	} else if (command == "move") {
		parse_move(args);
	} else if (command == "d") {
		print_board();
	} else {
		std::cout << "Invalid command, Use \"help\" to see a list of commands." << std::endl;
	}
}

void parse_position(std::stringstream &args)
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
		move_list.push_back(algbr_to_move(next));
	}
}

void parse_move(const std::stringstream &args)
{

}

void print_board()
{

}

CB::Move algbr_to_move(const std::string &algebraic)
{

}
