#pragma once

#include <string>
#include <vector>
#include <StartUI.h>
#include <ncurses.h>

class StartCLI : public StartUI
{
private:
	WINDOW *menu_win;
	int selected_row = 0;
	bool highlight = false;

	std::vector<std::string> player_choices {
		"Local",
		"Remote",
		"AI"
	};

	std::vector<std::string> timer_choices {
		"3 | 2",
		"10 | 0",
		"15 | 10",
		"30 | 0"
	};

	std::vector<std::vector<std::string>> menu_choices {player_choices, player_choices, timer_choices};
	std::string choice_labels[3] = {
		"White:",
		"Black:",
		"Time:"
	};

	struct menu_settings
	{
		int width;
		int height;
		int start_x;
		int start_y;
	};
	menu_settings my_menu;

public:
	StartCLI();
	virtual void init_menu();
	virtual void update_selected_row();
	virtual void switch_selected_row(int row_change);
	virtual void switch_selected_choice(int choice_change);
	virtual int handle_enter();
	virtual ~StartCLI();

	int selected_choices[3] = {0, 0, 0};
};