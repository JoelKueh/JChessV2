#include "GameGUI.h"

GameGUI::GameGUI()
{

}

void GameGUI::init_ui()
{

}

GameUI::state GameGUI::update_ui()
{
	state ui_state;
	ui_state.raw = 0;

	return ui_state;
}

int GameGUI::get_selected_piece()
{
	return -1;
}

void GameGUI::set_push_mask(uint64_t mask)
{

}

void GameGUI::set_cap_mask(uint64_t mask)
{

}

void GameGUI::set_special_mask(uint64_t mask)
{

}

void GameGUI::redraw_pieces()
{
	
}

void GameGUI::update_pieces(char board[8][8])
{

}

void GameGUI::update_time(int new_time, bool white)
{

}

GameGUI::~GameGUI() = default;
