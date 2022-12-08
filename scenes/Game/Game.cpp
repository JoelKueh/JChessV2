#include "Game.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

Game::Game(int white, int black, std::string *time_str)
{
  p_white = create_player(white);
  p_black = create_player(black);
  parse_time_str(time_str);

  board = new ChessBoard();
}

int Game::update()
{
	refresh();
	return update_states::M_OK;
}

void Game::update_pieces(char **board)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			mvwaddch(board_win, 2 * (row + 1) - 1, 4 * (col + 1) - 2, board[row][col]);
		}
	}
}

void Game::init()
{ 
	init_board_win();
  init_timer_win();
  init_input_win();
	refresh();
}

void Game::init_board_win()
{
	init_coords();
	refresh();
  board_win = newwin(17, 33, 2, 4);
  draw_chess_board();

	char **board_str = board->board_to_strarr();
	update_pieces(board_str);
	delete board_str;
	
	wrefresh(board_win);
}

void Game::init_coords()
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

void Game::draw_chess_board()
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
	wprintw(board_win, "%ls",output_str);
}

void Game::draw_piece_line()
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
	wprintw(board_win, "%ls",output_str);
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
	wprintw(board_win, "%ls",output_str);
}

void Game::init_timer_win()
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

void Game::init_input_win()
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
	return new_scene;
}

Game::~Game()
{
	debug_out << "Game Destructed" << std::endl;
}