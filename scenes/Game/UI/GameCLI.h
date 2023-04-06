#pragma once

#include "GameUI.h"
#include "../../../Global.h"
#include <ncurses.h>

class GameCLI : public GameUI
{
private:
	char board_str[8][8] = { 0 };
	uint64_t push_mask;
	uint64_t cap_mask;
	uint64_t special_mask;

	int cursor_col = 0;
	int cursor_row = 0;

	const int SELECT_COLOR = 20;
	const int PUSH_COLOR = 21;
	const int CAPTURE_COLOR = 22;
	const int SPECIAL_COLOR = 23;

	const int SELECT_INV = 24;
	const int PUSH_INV = 25;
	const int CAPTURE_INV = 26;
	const int SPECIAL_INV = 27;

	const int INV_OFFSET = 4;

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
	virtual int get_selected_piece();
	virtual void set_push_mask(uint64_t mask);
	virtual void set_cap_mask(uint64_t mask);
	virtual void set_special_mask(uint64_t mask);
	virtual void redraw_pieces();
	virtual void update_pieces(char **board);
	virtual void update_time(int new_time, bool white);
	virtual ~GameCLI();
};