#include "GameCLI.h"

GameCLI::GameCLI()
{
	init_board_win();
  	init_timer_win();
  	init_input_win();
	refresh();
}

void GameCLI::init_ui()
{

}

int GameCLI::update_ui()
{
	return 1;
}

void GameCLI::update_pieces(char **board)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			mvwaddch(board_win, 2 * (row + 1) - 1, 4 * (col + 1) - 2, board[row][col]);
		}
	}
	wrefresh(board_win);
}

void GameCLI::init_board_win()
{
	init_coords();
	refresh();
  	board_win = newwin(17, 33, 2, 4);
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