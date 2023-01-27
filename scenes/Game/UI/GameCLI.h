#pragma once

#include "GameUI.h"

class GameCLI : public GameUI
{
public:
	GameCLI();
	virtual int update_ui();
	virtual int update_pieces();
	virtual int update_time(int new_time, bool white);
	virtual ~GameCLI();
};