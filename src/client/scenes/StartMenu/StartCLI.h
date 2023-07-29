#pragma once

#include "StartUI.h"
#define ctrl(x)   ((x) & 0x1f)

#include <string>
#include <vector>
#include <ncurses.h>

extern int scr_x;
extern int scr_y;

/**
 * The CLI derivation for the Start Menu UI
*/
class StartCLI : public StartUI
{
private:
	WINDOW *menu_win;
	bool highlight = false;

	virtual void update_selected_row();
	virtual void switch_selected_row(int row_change);
	virtual void switch_selected_choice(int choice_change);
	virtual int handle_enter();

public:
	StartCLI();
	virtual void draw();
	virtual int update();
	virtual void init_menu();
	virtual std::string *get_time_str();
	virtual ~StartCLI();

	int selected_choices[3] = {0, 0, 0};
};
