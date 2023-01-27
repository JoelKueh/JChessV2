#pragma once

#include <ncurses.h>

class GameUI
{
public:
	GameUI();
	virtual int update_ui() = 0;
	virtual int update_pieces() = 0;
	virtual int update_time(int new_time, bool white) = 0;
	virtual ~GameUI() = 0;
};