#pragma once

#include <vector>
#include <string>

/**
 * Base class for the user interface for the StartMenu
 * Contains vectors to store the choices that the player can make as
 * well as virtual functions that allow the derived class to define the input output mechanism for those choices.
*/
class StartUI
{
protected:
	StartUI();

	virtual void update_selected_row() = 0;
	virtual void switch_selected_row(int row_change) = 0;
	virtual void switch_selected_choice(int choice_change) = 0;
	virtual int handle_enter() = 0;

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
	enum UI_STATE
	{
		U_ERR = -1,
		U_TERMINATE  = 0,
		U_OK = 1,
		U_RECREATE_REQUEST = 2,
	};
	int selected_choices[3] = { 0, 0, 0 };
	int selected_row = 0;

	/**
	 * Called directly from update.
	 * Something of a main function for the StartUI class.
	*/
	virtual int update() = 0;
	virtual void init_menu() = 0;
	virtual std::string *get_time_str() = 0;
	virtual ~StartUI();
};
