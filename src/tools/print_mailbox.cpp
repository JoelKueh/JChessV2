#include <iostream>
#include <cstdint>

bool sq_is_white(int sq, uint64_t color)
{
	return ((1ULL << sq) & color);
}

char sq_to_char(int sq, int mailbox[64], uint64_t color)
{
	// this is not null-character delimited, don't try to print it
	const char piece_arr[7] = { 'p', 'n', 'b', 'r', 'q', 'k', ' ' };
	constexpr int to_up = 'a' - 'A';

	char piece = piece_arr[mailbox[sq]];
	piece -= sq_is_white(sq, color) ? to_up : 0;

	return piece;
}

const char np_line[] = "+---+---+---+---+---+---+---+---+";
void print_board(char board_str[8][8])
{
	for (int row = 0; row < 8; ++row) {
		std::cout << np_line << std::endl << "| ";
		for (int col = 0; col < 8; ++col) {
			std::cout << board_str[row][col] << " | ";
		}
		std::cout << std::endl;
	}
	std::cout << np_line << std::endl;
}

int main(int argc, char *argv[])
{
	int mailbox[64] = {3, 6, 6, 6, 5, 6, 6, 3, 0, 6, 0, 0, 4, 0, 2, 6, 2, 1, 6, 6, 0, 1, 0, 6, 
    6, 6, 6, 0, 1, 6, 6, 6, 6, 0, 6, 6, 0, 6, 6, 6, 6, 6, 1, 6, 6, 4, 6, 0, 0, 0, 0, 2, 2, 0, 0, 0, 3, 6, 6, 6, 5, 6, 6, 3};
	char board[8][8];
	uint64_t color = 10520166906101497856ULL;

	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			board[row][col] = sq_to_char(row * 8 + col, mailbox, color);
		}
	}

	print_board(board);

	return 0;
}
