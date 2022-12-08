#pragma once

#include <vector>
#include <string>

class StartUI
{
private:

public:
	StartUI();
	virtual void init_menu() = 0;
	virtual void update_selected_row() = 0;
	virtual void switch_selected_row(int row_change) = 0;
	virtual void switch_selected_choice(int choice_change) = 0;
	virtual int handle_enter() = 0;
	virtual ~StartUI() = 0;
};