#include "GameCLI.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

GameCLI::GameCLI()
{
	init_board_win();
  	init_timer_win();
  	init_input_win();
	refresh();

	// TODO: MOVE THIS
	init_ui();
}

void GameCLI::init_ui()
{
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

int GameCLI::update_ui()
{
	int state = U_OK;

	bool do_redraw = false;
	int input = wgetch(board_win);
	switch (input) {
	case KEY_DOWN:
		cursor_row += 1;
		cursor_row = cursor_row > 7 ? 7 : cursor_row;
		do_redraw = true;
		break;
	case KEY_UP:
		cursor_row -= 1;
		cursor_row = cursor_row < 0 ? 0 : cursor_row;
		do_redraw = true;
		break;
	case KEY_RIGHT:
		cursor_col += 1;
		cursor_col = cursor_col > 7 ? 7 : cursor_col;
		do_redraw = true;
		break;
	case KEY_LEFT:
		cursor_col -= 1;
		cursor_col = cursor_col < 0 ? 0 : cursor_col;
		do_redraw = true;
		break;
	case ' ':
	case 'P':
	case 'p':
	case 'N':
	case 'n':
	case 'B':
	case 'b':
	case 'R':
	case 'r':
	case 'Q':
	case 'q':
	case 'K':
	case 'k':
		state = U_WRITE;
		write_buf.piece = input;
		write_buf.sq = cursor_row * 8 + cursor_col;
		break;
	defualt:
		do_redraw = false;
		break;
	}

	if (do_redraw) {
		redraw_pieces();
	}

	return state;
}

void GameCLI::update_pieces(char **board)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			board_str[row][col] = board[row][col];
		}
	}
	redraw_pieces();
}

void GameCLI::redraw_pieces()
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int color = -1;
			if (1ULL << (row * 8 + col) & push_mask)
				color = PUSH_COLOR;

			if (row == cursor_row && col == cursor_col)
				color = SELECT_COLOR;
			
			if (color != -1) {
				wattron(board_win, COLOR_PAIR(color + INV_OFFSET));
				mvwaddwstr(board_win, 2 * row + 1, 4 * col + 1, r_half_str);
				wattron(board_win, COLOR_PAIR(color));
				waddch(board_win, board_str[row][col]);
				wattron(board_win, COLOR_PAIR(color + INV_OFFSET));
				waddwstr(board_win, l_half_str);
			} else {
				mvwaddch(board_win, 2 * row + 1, 4 * col + 1, ' ');
				waddch(board_win, board_str[row][col]);
				waddch(board_win, ' ');
			}

			wattroff(board_win, COLOR_PAIR(color + INV_OFFSET));
		}
	}
	wrefresh(board_win);
}

int GameCLI::get_selected_piece()
{
	return cursor_row * 8 + cursor_col;
}

void GameCLI::set_push_mask(uint64_t mask)
{
	push_mask = mask;
}

void GameCLI::set_cap_mask(uint64_t mask)
{
	cap_mask = mask;
}

void GameCLI::set_special_mask(uint64_t mask)
{
	special_mask = mask;
}

void GameCLI::init_board_win()
{
	init_coords();
	refresh();
  	board_win = newwin(17, 33, 2, 4);
	wtimeout(board_win, 500);
	keypad(board_win, TRUE);
  	draw_chess_board();
	
	wrefresh(board_win);
}

void GameCLI::init_timer_win()
{
  	timer_win = newwin(5, 9, 8, 41);

	wchar_t output_str[10] = { 0 };
	output_str[9] = '\0';
	output_str[0] = d_r;
	for(int col = 1; col < 8; col++)
	{
		output_str[col] = hz_ln;
	}
	output_str[8] = d_l;
	wprintw(timer_win,"%ls",output_str);
	
	for(int col = 0; col < 8; col++)
	{
		output_str[col] = ' ';
	}
	output_str[0] = vr_ln;
	output_str[8] = vr_ln;
	wprintw(timer_win, "%ls", output_str);

	output_str[0] = du_r;
	for(int col = 1; col < 8; col++)
	{
		output_str[col] = hz_ln;
	}
	output_str[8] = du_l;
	wprintw(timer_win,"%ls",output_str);

	for(int col = 0; col < 8; col++)
	{
		output_str[col] = ' ';
	}
	output_str[0] = vr_ln;
	output_str[8] = vr_ln;
	wprintw(timer_win, "%ls", output_str);

	output_str[0] = u_r;
	for(int col = 1; col < 8; col++)
	{
		output_str[col] = hz_ln;
	}
	output_str[8] = u_l;
	wprintw(timer_win,"%ls",output_str);

	wrefresh(timer_win);
}

void GameCLI::init_input_win()
{

}

void GameCLI::init_coords()
{
	for (int i = 0; i < 8; i++)
	{
		char let = 'h' - i;
		char num = '1' + i;
		mvaddch(3 + 2 * i, 2, let);
		mvaddch(3 + 2 * i, 38, let);
		mvaddch(1, 6 + 4 * i, num);
		mvaddch(19, 6 + 4 * i, num);
	}
}

void GameCLI::draw_chess_board()
{
	draw_top_line();
	for(int row = 1; row < 16; row++)
	{
		if(row % 2 == 0)
		{
			draw_other_line();
		}
		else
		{
			draw_piece_line();
		}
	}
	draw_bottom_line();
}

void GameCLI::draw_top_line()
{
	wchar_t output_str[34] = { 0 };
	output_str[0] = d_r;
	for(int col = 1; col < 32; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = d_lr;
		}
		else
		{
			output_str[col] = hz_ln;
		}
	}
	output_str[32] = d_l;
	output_str[33]='\0';
	wprintw(board_win, "%ls",output_str);
}

void GameCLI::draw_piece_line()
{
	wchar_t output_str[34] = { 0 };
	for(int col = 0; col < 33; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = vr_ln;
		}
		else
		{
			output_str[col] = ' ';
		}
	}
	output_str[33]='\0';
	wprintw(board_win, "%ls",output_str);
}

void GameCLI::draw_other_line()
{
	wchar_t output_str[34] = { 0 };
	output_str[0] = du_r;
	for(int col = 1; col < 32; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = du_lr;
		}
		else
		{
			output_str[col] = hz_ln;
		}
	}
	output_str[32] = du_l;
	output_str[33]='\0';
	wprintw(board_win, "%ls",output_str);
}

void GameCLI::draw_bottom_line()
{
	wchar_t output_str[34] = { 0 };
	output_str[0] = u_r;
	for(int col = 1; col < 32; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = u_lr;
		}
		else
		{
			output_str[col] = hz_ln;
		}
	}
	output_str[32] = u_l;
	output_str[33]='\0';
	wprintw(board_win, "%ls",output_str);
}

void GameCLI::update_time(int new_time, bool white)
{

}

GameCLI::~GameCLI() = default;