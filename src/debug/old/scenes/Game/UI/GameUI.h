#pragma once

#include <ncurses.h>

class GameUI
{
public:

	union state {
		uint16_t raw;
		struct {
			bool ok: 1;
			bool write: 1;
			bool do_redraw: 1;

			bool cursor_moved: 1;
			bool update_mask: 1;

			bool mk_move: 1;
			bool unmk_move: 1;
		};
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

	GameUI();
	virtual void init_ui() = 0;
	virtual state update_ui() = 0;
	virtual void update_pieces(char board[8][8]) = 0;
	virtual int get_selected_piece() = 0;
	virtual void set_push_mask(uint64_t mask) = 0;
	virtual void set_cap_mask(uint64_t mask) = 0;
	virtual void set_special_mask(uint64_t mask) = 0;
	virtual void redraw_pieces() = 0;
	virtual void update_time(int new_time, bool white) = 0;
	virtual ~GameUI() = 0;

	move read_move_buf();
	void reset_move_buf();

protected:
	move move_buf;
};
