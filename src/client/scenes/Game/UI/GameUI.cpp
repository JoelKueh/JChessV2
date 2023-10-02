#include "GameUI.h"

GameUI::GameUI() = default;
GameUI::~GameUI() = default;

GameUI::move GameUI::read_move_buf()
{
	return move_buf;	
}

void GameUI::reset_move_buf()
{
	move_buf.start_sq = -1;
	move_buf.end_sq = -1;
}
