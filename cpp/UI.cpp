#include "UI.h"

static inline bool is_even(int x) { return x % 2; }

UI::UI()
{
	setlocale(LC_CTYPE, "");
	initscr();
}


void UI::cleanup()
{
	endwin();
}

void UI::wait_key_press()
{
	getch();
}

UnicodeUI::UnicodeUI() = default;

void UnicodeUI::draw_chess_board(int start_x, int start_y, char** board)
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
			draw_piece_line(0, nullptr);
		}
	}
	draw_bottom_line();
}

void UnicodeUI::draw_top_line()
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
	printw("%ls\n",output_str);
}

void UnicodeUI::draw_piece_line(int row_num, char* row_arr)
{
	wchar_t output_str[34] = { 0 };
	for(int col = 0; col < 33; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = vr_ln;
		}
		else if(col % 4 == 2)
		{
			output_str[col] = row_arr[row_num];
		}
		else
		{
			output_str[col] = ' ';
		}
	}
	output_str[33]='\0';
	printw("%ls\n",output_str);
}

void UnicodeUI::draw_other_line()
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
	printw("%ls\n",output_str);
}

void UnicodeUI::draw_bottom_line()
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
	printw("%ls\n",output_str);
}

NoUnicodeUI::NoUnicodeUI() = default;

void NoUnicodeUI::draw_chess_board(int start_x, int start_y, char** board)
{
	for(int row = 0; row < 17; row++)
	{
		if(row % 2 == 0)
		{
			draw_other_line();
		}
		else
		{
			draw_piece_line(0, nullptr);
		}
	}
}

void NoUnicodeUI::draw_other_line()
{
	char output_str[34] = { 0 };
	for(int col = 0; col < 33; col++)
	{
		if(col % 4 == 0)
		{
			output_str[col] = '+';
		}
		else
		{
			output_str[col] = '-';
		}

	}
	output_str[33]='\0';
	printw("%s\n",output_str);
}

void NoUnicodeUI::draw_piece_line(int row_num, char* row_arr)
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
