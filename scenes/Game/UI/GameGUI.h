#pragma once

#include "GameUI.h"

class GameGUI : public GameUI
{
public:
	GameGUI();
	virtual int update_ui();
	virtual int update_pieces();
	virtual int update_time(int new_time, bool white);
	virtual ~GameGUI();
};