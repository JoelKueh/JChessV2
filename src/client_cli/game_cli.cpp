
#include "game_cli.h"

game_cli::game_cli()
{
	wipe_board();

	init_board_win();
  	init_timer_win();
  	init_input_win();
	refresh();

	move_buf.start_sq = -1;
	move_buf.end_sq = -1;

	init_color(SELECT_COLOR, 500, 500, 500);
	init_color(PUSH_COLOR, 447, 651, 879);
	init_color(CAPTURE_COLOR, 961, 365, 365);
	init_color(SPECIAL_COLOR, 929, 913, 427);

	init_pair(SELECT_COLOR, COLOR_BLACK, COLOR_WHITE);
	init_pair(PUSH_COLOR, COLOR_BLACK, PUSH_COLOR);
	init_pair(CAPTURE_COLOR, COLOR_BLACK, CAPTURE_COLOR);
	init_pair(SPECIAL_COLOR, COLOR_BLACK, SPECIAL_COLOR);

	init_pair(SELECT_INV, COLOR_WHITE, -1);
	init_pair(PUSH_INV, PUSH_COLOR, -1);
	init_pair(CAPTURE_INV, CAPTURE_COLOR, -1);
	init_pair(SPECIAL_INV, SPECIAL_COLOR, -1);
}

void game_cli::wipe_board()
{
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			board_str[row][col] = ' ';
		}
	}
}

void game_cli::set_cpos(int row, int col)
{
	cpos[0] = row;
	cpos[1] = col;
}

void game_cli::set_row(int row)
{
	cpos[0] = row;
}

void game_cli::set_col(int col)
{
	cpos[1] = col;
}

int game_cli::get_row()
{
	return cpos[0];
}

int game_cli::get_col()
{
	return cpos[1];
}
