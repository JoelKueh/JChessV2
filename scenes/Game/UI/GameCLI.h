#pragma once

#include "GameUI.h"
#include "../../../Global.h"
#include <ncurses.h>

class GameCLI : public GameUI
{
private:
  	WINDOW *board_win;
  	WINDOW *timer_win;
  	WINDOW *input_win;
  	void init_board_win();
	void init_timer_win();
	void init_input_win();
  	void init_coords();
  
  	void update_piece_row(char *row);
  	void update_piece_col(char *col);
  	void update_piece_square(char square);
  	void draw_chess_board();
  	void draw_top_line();
	void draw_piece_line();
	void draw_other_line();
	void draw_bottom_line();

public:
	GameCLI();
	virtual void init_ui();
	virtual int update_ui();
	virtual void update_pieces(char **board);
	virtual void update_time(int new_time, bool white);
	virtual ~GameCLI();
};