#pragma once

#include <GLFW/glfw3.h>
#include "StartUI.h"

/**
 * Not fully implemented yet, a windows is created, but nothing is ever drawn.
*/
class StartGUI : public StartUI
{
	virtual void update_selected_row();
	virtual void switch_selected_row(int row_change);
	virtual void switch_selected_choice(int choice_change);
	virtual int handle_enter();

public:
	StartGUI();
	virtual int handle_nav();
	virtual void init_menu();
	virtual std::string *get_time_str();
	virtual ~StartGUI();

	int selected_choices[3] = {0, 0, 0};
};