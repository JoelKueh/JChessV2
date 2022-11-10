#include "UI.h"

static inline bool is_even(int x) { return x % 2; }

Graphics::Graphics()
{
	setlocale(LC_CTYPE, "");
	initscr();
}

void Graphics::draw_chess_board(int start_x, int start_y)
{
	for(int row = 0; row < 17; row++)
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
}

void Graphics::draw_other_line()
{
	wchar_t output_str[34] = { 0 };
	for(int col = 0; col < 33; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = '+';
		}
		else
		{
			output_str[col] = ;
		}

	}
	output_str[33]='\0';
	printw("%ls\n",output_str);
}

void Graphics::draw_piece_line()
{
	char output_str[34] = "";
	for(int col = 0; col < 33; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = '|';
		}
		else if(col % 4 == 2)
		{
			output_str[col] = 'P';
		}
		else
		{
			output_str[col] = ' ';
		}
	}
	output_str[33]='\0';
	printw("%s\n",output_str);
}

void Graphics::cleanup()
{
	endwin();
}

void Graphics::wait_key_press()
{
	getch();
}
