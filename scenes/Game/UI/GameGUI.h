#pragma once

#include "GameUI.h"

class GameGUI : public GameUI
{
public:
	GameGUI();
	virtual void init_ui();
	virtual int update_ui();
	virtual void update_pieces(char **board);
	virtual int get_selected_piece();
	virtual void set_highlight_mask(uint64_t mask);
	virtual void update_time(int new_time, bool white);
	virtual ~GameGUI();
};