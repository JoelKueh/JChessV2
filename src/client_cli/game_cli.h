
#ifndef H_GAME_CLI
#define H_GAME_CLI

#include <ncurses.h>

class game_cli
{
private:
	char board_str[8][8];
	uint64_t push_mask;
	uint64_t cap_mask;
	uint64_t special_mask;

	const int SELECT_COLOR = 20;
	const int PUSH_COLOR = 21;
	const int CAPTURE_COLOR = 22;
	const int SPECIAL_COLOR = 23;
	const int SELECT_INV = 24;
	const int PUSH_INV = 25;
	const int CAPTURE_INV = 26;
	const int SPECIAL_INV = 27;
	const int INV_OFFSET = 4;

	int cpos[2];
	int mvbuf[4];

	void wipe_board();

public:
	game_cli();

	void init();
	void reset();

	void set_cpos(int row, int col);
	void set_row(int row);
	void set_col(int col);
	int get_row();
	int get_col();
	void draw();
	~game_cli();
};

#endif
