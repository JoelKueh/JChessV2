#pragma once

#include <ncurses.h>

class GameUI
{
public:
	GameUI();
	virtual void init_ui() = 0;
	virtual int update_ui() = 0;
	virtual void update_pieces(char **board) = 0;
	virtual void update_time(int new_time, bool white) = 0;
	virtual ~GameUI() = 0;
};