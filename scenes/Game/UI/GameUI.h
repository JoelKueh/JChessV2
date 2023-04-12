#pragma once

#include <ncurses.h>

class GameUI
{
private:

public:

	enum state {
		U_OK = 0,
		U_WRITE = 1,
		U_MOVE = 2,
		U_UPDATE_MASK = 3,
	};

	struct write {
		char piece;
		int sq;
	};

	struct move {
		int start_sq;
		int end_sq;
	};

	write write_buf;
	move move_buf;

	GameUI();
	virtual void init_ui() = 0;
	virtual int update_ui() = 0;
	virtual void update_pieces(char board[8][8]) = 0;
	virtual int get_selected_piece() = 0;
	virtual void set_push_mask(uint64_t mask) = 0;
	virtual void set_cap_mask(uint64_t mask) = 0;
	virtual void set_special_mask(uint64_t mask) = 0;
	virtual void redraw_pieces() = 0;
	virtual void update_time(int new_time, bool white) = 0;
	virtual ~GameUI() = 0;
};