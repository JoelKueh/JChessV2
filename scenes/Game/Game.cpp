#include "Game.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

Game::Game(int white, int black, std::string *time_str)
{
	// Create the players who will be giving the moves
  	p_white = create_player(white);
  	p_black = create_player(black);
  	parse_time_str(time_str);

	// Create the board to hold the pieces and moves.
  	board = new ChessBoard();
	if (my_options.gui)
		UI = new GameGUI();
	else
		UI = new GameCLI();

	char **board_str = board->board_to_strarr();
	UI->update_pieces(board_str);
	delete board_str;
}

void Game::init()
{ 

}

int Game::update()
{
	// Handle general updates to the UI.
	refresh();
	UI->update_ui();

	// Handle updates to the time counts for each player.
	auto time_s = std::chrono::system_clock::now();
	auto time_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time_s);
	unsigned long long time_now = time_ms.time_since_epoch().count();
	if(board->white_turn)
	{
		p_white->time = time_now - last_time;
		UI->update_time(p_white->time / 1000, true);
	}
	else
	{
		p_black->time = time_now - last_time;
		UI->update_time(p_white->time / 1000, false);
	}
	last_time = time_now;

	// TODO: Handle game state checking.


	// TODO: Handle player move checking.

	// TODO:

	return update_states::M_OK;
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

void Game::update_player_time()
{
	// Handle updates to the time counts for each player.
	auto time_s = std::chrono::system_clock::now();
	auto time_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time_s);
	ulong time_now = time_ms.time_since_epoch().count();
	if(board->white_turn)
	{
		p_white->time = time_now - last_time;
		UI->update_time(p_white->time / 1000, true);
	}
	else
	{
		p_black->time = time_now - last_time;
		UI->update_time(p_white->time / 1000, false);
	}
	last_time = time_now;
}

void handle_player_move()
{
	
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