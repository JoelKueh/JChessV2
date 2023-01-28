#pragma once

#include "GameUI.h"

class GameGUI : public GameUI
{
public:
	GameGUI();
	virtual void init_ui();
	virtual int update_ui();
	virtual void update_pieces(char **board);
	virtual void update_time(int new_time, bool white);
	virtual ~GameGUI();
};