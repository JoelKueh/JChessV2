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
  	board = new CB::BoardRep();
	if (my_options.gui)
		UI = new GameGUI();
	else
		UI = new GameCLI();

	//char fen_str[] = "rnbqkbnr/pppp1ppp/4p3/8/Q7/2P5/PP1PPPPP/RNB1KBNR b KQkq - 1 2";
	char fen_str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	board->write_fen(fen_str);

	char board_str[8][8];
	board->board_to_str(board_str);
	UI->update_pieces(board_str);
}

void Game::init()
{ 

}

int Game::update()
{
	// Handle general updates to the UI.
	refresh();
	GameUI::state ui_state = UI->update_ui();

	if (ui_state.update_mask) {
		int selected = UI->get_selected_piece();
		CB::move_set moves;
		board->get_mv_set(&moves, selected);
		UI->set_push_mask(moves.push);
		UI->set_cap_mask(moves.cap);
		UI->set_special_mask(moves.special);
		UI->redraw_pieces();
		last_selected_piece = selected;
	}

	if (ui_state.mk_move) {
		GameUI::move move = UI->read_move_buf();
		UI->reset_move_buf();

		CB::pid promo_pid;
		switch (move.promo_piece) {
			case 'n': promo_pid = CB::KNIGHT; break;
			case 'b': promo_pid = CB::BISHOP; break;
			case 'r': promo_pid = CB::ROOK; break;
			case 'q': promo_pid = CB::QUEEN; break;
			default: break;
		}
		CB::Move move_full = board->format_mv(
				move.end_sq, move.start_sq, promo_pid);

		if (!move_full.is_invalid()) {
			board->make(move_full);
		
			char board_str[8][8];
			board->board_to_str(board_str);
			UI->update_pieces(board_str);

			int selected = UI->get_selected_piece();
			CB::move_set moves;
			board->get_mv_set(&moves, selected);
			UI->set_push_mask(moves.push);
			UI->set_cap_mask(moves.cap);
			UI->set_special_mask(moves.special);
			UI->redraw_pieces();
		}
	}	

	if (ui_state.unmk_move) {
		board->unmake();

		char board_str[8][8];
		board->board_to_str(board_str);
		UI->update_pieces(board_str);

		int selected = UI->get_selected_piece();
		CB::move_set moves;
		board->get_mv_set(&moves, selected);
		UI->set_push_mask(moves.push);
		UI->set_cap_mask(moves.cap);
		UI->set_special_mask(moves.special);
		UI->redraw_pieces();
	}


	// Handle updates to the time counts for each player.
	update_player_time();

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
	if(board->is_white_turn())
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
