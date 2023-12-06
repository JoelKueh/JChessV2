
#include "net_utils.h"

int split_command(char command[], char ***words)
{
	// Count words in command and allocate an array of start positions.
	int num_words = 1;
	char *c = command;
	while (*c) { num_words += (*c++ == '|' ? 1 : 0); }
	*words = new char*[num_words];

	// Step through command character by character, when you come across
	// a '|', replace the '|' with a '\0' and set the next 
	int word_index = 0, i = 0;
	char ch;
	(*words)[word_index] = command;
	while (word_index < num_words - 1) {
		if ((*words)[word_index][i] == '|') {
			(*words)[word_index][i] = '\0';
			(*words)[word_index + 1] = (*words)[word_index] + i + 1;
			word_index++;
			i = 0;
		}
		++i;
	}
	return num_words;
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

