#include "Game.h"

// DEBUG
#include <iostream>

Game::Game(int white, int black, std::string *time_str)
{
  p_white = create_player(white);
  p_black = create_player(black);
  parse_time_str(time_str);

  board = new ChessBoard();

  init_scene();
}

void Game::init_scene()
{
  erase();
  init_board_win();
  init_timer_win();
  init_input_win();
}

void Game::init_board_win()
{
  board_win = newwin(16, 34, 0, 0);
	box(board_win, 0, 0);
  draw_chess_board(board->board_to_strarr());
}

void Game::draw_chess_board(char **board_str)
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
			draw_piece_line(0, board_str[(row - 1) / 2]);
		}
	}
	draw_bottom_line();
}

void Game::draw_top_line()
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
	wprintw(board_win, "%ls\n",output_str);
}

void Game::draw_piece_line(int row_num, char* row_arr)
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
	wprintw(board_win, "%ls\n",output_str);
}

void Game::draw_other_line()
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
	wprintw(board_win, "%ls\n",output_str);
}

void Game::draw_bottom_line()
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
	wprintw(board_win, "%ls\n",output_str);
}

void Game::init_timer_win()
{

}

void Game::init_input_win()
{

}

int Game::update()
{

}

Player *Game::create_player(int type)
{
  switch (type)
  {
    default:
      return new PlayerLocal();
    case player_type::remote:
      return new PlayerRemote();
    case player_type::AI:
      return new PlayerAI();
    case player_type::TheFish:
      return new PlayerTheFish();
  }
}

void Game::parse_time_str(std::string *time_str)
{
  int bar_location;
  for (int i = 0; i < time_str->size(); i++)
  {
    if ((*time_str)[i] == '|')
    {
      bar_location = i;
      break;
    }
  }

  int time = std::stoi(time_str->substr(0, bar_location - 1));
  p_white->time = time;
  p_black->time = time;

  increment = std::stoi(time_str->substr(bar_location + 2, time_str->size() - bar_location - 2));
}

Scene* Game::create_new()
{
	Scene *new_scene = new StartMenu();
}