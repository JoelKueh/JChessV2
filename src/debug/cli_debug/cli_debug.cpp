#include <chrono>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include "../../common/CB_2_0/BoardRep.cpp"

// TODO: IMPLEMENT MULTITHREADED THINKING
#include <thread>

#define BYTE_TO_BINARY(byte)  \
  (byte & 0x01 ? '1' : '0') << \
  (byte & 0x02 ? '1' : '0') << \
  (byte & 0x04 ? '1' : '0') << \
  (byte & 0x08 ? '1' : '0') << \
  (byte & 0x10 ? '1' : '0') << \
  (byte & 0x20 ? '1' : '0') << \
  (byte & 0x40 ? '1' : '0') << \
  (byte & 0x80 ? '1' : '0')


#define U64_TO_BB(file, u64)  \
  file << BYTE_TO_BINARY(u64) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 8) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 16) << std::endl \
	<< BYTE_TO_BINARY(u64 >> 24) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 32) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 40) << std::endl \
  	<< BYTE_TO_BINARY(u64 >> 48) << std::endl \
    << BYTE_TO_BINARY(u64 >> 56) << std::endl << std::endl \

void parse_command(CB::BoardRep &board, const std::string &command, std::stringstream &args);
void parse_position(CB::BoardRep &board, std::stringstream &args);
void parse_move(CB::BoardRep &board, std::stringstream &args);
void print_move(CB::Move &move);
void print_moves(CB::BoardRep &board, std::stringstream &args);
void print_board(const CB::BoardRep &board);
void handle_go(CB::BoardRep &board, std::stringstream &args);
std::vector<CB::Move> *gen_mv_list_slow(CB::BoardRep &board);
void init_perft(CB::BoardRep &board, int depth);
uint64_t perft(CB::BoardRep &board, int depth);
std::string to_algebraic(int sq);
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

void parse_command(CB::BoardRep &board, const std::string &command, std::stringstream &args)
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
	} else if (command == "movels") {
		print_moves(board, args);
	} else if (command == "go") {
		handle_go(board, args);
	} else {
		std::cout << "Invalid command, Use \"help\" to see a list of commands." << std::endl;
	}
}

void parse_position(CB::BoardRep &board, std::stringstream &args)
{
	std::string fen;
	std::string next;

	args >> next;
	args >> next;
	while (args && next != "move") {
		fen += next;
		fen.push_back(' ');

		args >> next;
	}

	const char *fen_c = fen.c_str();
	board.write_fen(fen_c);
 	CB::Move move;
	while (args >> next) {
		move = algbr_to_move(board, next);
		if (move.is_invalid()) {
			std::cout << "INVALID MOVE, SKIPPING MOVE EXECUTION";
			std::cout << std::endl;
			return;
		}
		board.make(move);
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

void print_moves(CB::BoardRep &board, std::stringstream &args)
{
	std::string square;
	if (args >> square) {
		if (square.size() != 2) {
			std::cout << "INVALID ARGS, SKIPPING" << std::endl;
			return;
		}

		square.at(0) = tolower(square.at(0));
		square.at(1) = tolower(square.at(1));

		int sq = (square.at(0) - 'a')
			+ (7 - (square.at(1) - '1')) * 8;

		CB::move_set moves;
		board.get_mv_set(&moves, sq);

		std::cout << "Quiets:\n";
		U64_TO_BB(std::cout, moves.push);
		std::cout << "Cap:\n";
		U64_TO_BB(std::cout, moves.cap);
		std::cout << "Special:\n";
		U64_TO_BB(std::cout, moves.special);
		std::cout << "Promo:\n";
		U64_TO_BB(std::cout, moves.promo);
		std::cout << std::flush;

		return;
	}

	// TODO: FIX THIS
	/*CB::MoveList move_list;
	board.gen_move_list(&move_list);
	std::cout << "Num Moves: " << move_list.size() << std::endl;
	while (!move_list->empty()) {
		CB::Move &move = move_list->back();

		print_move(move);
		std::cout << std::endl;

		move_list->pop_back();
	}

	delete move_list;*/
}

void print_move(CB::Move &move)
{
	std::cout << (char)('a' + move.get_from() % 8)
		<< (char)('8' - move.get_from() / 8);
	std::cout << (char)('a' + move.get_to() % 8)
		<< (char)('8' - move.get_to() / 8);
}

void handle_go(CB::BoardRep &board, std::stringstream &args)
{
	std::string command;
	args >> command;

	for (int i = 0; i < command.size(); ++i) {
		command.at(i) = tolower(command.at(i));
	}
	
	if (command == "perft") {
		int depth;
		args >> depth;
		init_perft(board, depth);
	} else {
		std::cout << "INVALID ARGUMENTS" << std::endl;
		return;
	}
}

// This function is a little bit odd, it was only used once to run a perft
// test on CB::get_mv_set()
std::vector<CB::Move> *gen_mv_list_slow(CB::BoardRep &board)
{
	std::vector<CB::Move> *mv_set = new std::vector<CB::Move>;

	for (int from = 0; from < 64; ++from) {
		CB::move_set mask;
		board.get_mv_set(&mask, from);

		while(mask.push) {
			int to = CB::pop_rbit(mask.push);
			mv_set->push_back(board.format_mv(to, from, CB::pid::EMPTY));
		}

		while(mask.cap) {
			int to = CB::pop_rbit(mask.cap);
			mv_set->push_back(board.format_mv(to, from, CB::pid::EMPTY));
		}

		while(mask.special) {
			int to = CB::pop_rbit(mask.special);
			mv_set->push_back(board.format_mv(to, from, CB::pid::EMPTY));
		}

		while(mask.promo) {
			int to = CB::pop_rbit(mask.promo);
			mv_set->push_back(board.format_mv(to, from, CB::pid::KNIGHT));
			mv_set->push_back(board.format_mv(to, from, CB::pid::BISHOP));
			mv_set->push_back(board.format_mv(to, from, CB::pid::ROOK));
			mv_set->push_back(board.format_mv(to, from, CB::pid::QUEEN));
		}
	}

	return mv_set;
}

void init_perft(CB::BoardRep &board, int depth)
{
	auto start = std::chrono::high_resolution_clock::now();

	uint64_t nodes = 0;

	CB::MoveList move_list;
	board.gen_move_list(&move_list);
	// DEBUG:
	// move_list = gen_mv_list_slow(board);
	for (int i = 0; i < move_list.size(); ++i) {
		std::cout << std::flush;

		board.make(move_list.at(i));
		uint64_t result = perft(board, depth - 1);
		nodes += result;
		board.unmake();

		std::cout << to_algebraic(move_list.at(i).get_from())
			<< to_algebraic(move_list.at(i).get_to());

		switch (move_list.at(i).get_flags()) {
			case CB::Move::KNIGHT_PROMO:
			case CB::Move::KNIGHT_PROMO_CAPTURE:
				std::cout << 'n';
				break;
			case CB::Move::BISHOP_PROMO:
			case CB::Move::BISHOP_PROMO_CAPTURE:
				std::cout << 'b';
				break;
			case CB::Move::ROOK_PROMO:
			case CB::Move::ROOK_PROMO_CAPTURE:
				std::cout << 'r';
				break;
			case CB::Move::QUEEN_PROMO:
			case CB::Move::QUEEN_PROMO_CAPTURE:
				std::cout << 'q';
				break;
			default:
				break;
		}
		std::cout << ": " << result << std::endl;
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Perft completed successfully with " << nodes
		<< " nodes searched " << std::endl
		<< "after " << duration.count() << " milliseconds." << std::endl;
}

uint64_t perft(CB::BoardRep &board, int depth)
{
	/* Faster but kind of cheating. (200M Leaf Nps)
	CB::MoveList move_list;
	uint64_t nodes = 0;
	board.gen_move_list(&move_list);
	if (depth == 0)
		return move_list.size();
	*/

	/* Slower but actually visits leaf nodes. (50M Leaf Nps)
	 * This should help when testing performance of alpha-beta search
	 * as it's a better representation of what actually happens.
	 * E.G. will make alpha-beta pruning stand out more (or at all for 
	 * that matter). */
	if (depth == 0) {
	        return 1ULL;
	}
	/* Marginal speed increase by avoiding move_list generation on leaf
	 * nodes. A further speed increase may be achieved by creating some
	 * tree walk move structure that relies on depth-first search and
	 * deepens as the search deepens.
	 *
	 * E.G. don't make a new list for every node when you only need one
	 * list for every level. The compiler may already be optimizing this,
	 * but it's worth a shot. */
	CB::MoveList move_list;
	uint64_t nodes = 0;
	board.gen_move_list(&move_list);

	for (int i = 0; i < move_list.size(); ++i) {
		board.make(move_list.at(i));
		nodes += perft(board, depth - 1);
		board.unmake();
	}
	return nodes;
}

std::string to_algebraic(int sq)
{
	std::string out;
	out.push_back('a' + sq % 8);
	out.push_back('8' - sq / 8);
	return out;
}
