#include "StartMenu.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

StartMenu::StartMenu()
{
	my_menu.width = 50;
	my_menu.height = 13;	

	my_menu.start_x = scr_x / 2 - my_menu.width / 2;
	my_menu.start_y = scr_y / 2 - my_menu.height / 2;

}

void StartMenu::init()
{
	init_menu();
}

int StartMenu::update()
{
	refresh();
	
	int input = wgetch(menu_win);
	switch (input)
	{
		case KEY_UP:
			switch_selected_row(-1);
			break;
		case KEY_DOWN:
			switch_selected_row(1);
			break;
		case KEY_LEFT:
			switch_selected_choice(-1);
			break;
		case KEY_RIGHT:
			switch_selected_choice(1);
			break;
		case ctrl('c'):
			return update_states::M_TERMINATE;
			break;
		case '\n':
			return handle_enter();
			break;
		default:
			break;
	}
	update_selected_row();

	wrefresh(menu_win);

	return update_states::M_OK;
}

void StartMenu::init_menu()
{
	menu_win = newwin(my_menu.height, my_menu.width, my_menu.start_y, my_menu.start_x);
	box(menu_win, 0, 0);

	wattron(menu_win, A_BOLD);
	wattron(menu_win, A_UNDERLINE);
	mvwaddstr(menu_win, 2, my_menu.width / 2 - 7, "Chess: The Game");
	wattroff(menu_win, A_BOLD);
	wattroff(menu_win, A_UNDERLINE);

	for (int i = 0; i < 3; i++)
	{
		int num_choices = menu_choices[i].size();
		mvwaddstr(menu_win, 4 + i * 2, 3, choice_labels[i].c_str());
		waddstr(menu_win, "   ");
		wattron(menu_win, A_STANDOUT);
		waddstr(menu_win, menu_choices[i][0].c_str());
		wattroff(menu_win, A_STANDOUT);
		for (int j = 1; j < num_choices; j++)
		{
			waddstr(menu_win, "  ");
			waddstr(menu_win, menu_choices[i][j].c_str());
		}
	}

	mvwaddstr(menu_win, 10, my_menu.width / 2 - 4, "[Start]");
	wtimeout(menu_win, 500);
	keypad(menu_win, TRUE);
}

void StartMenu::update_selected_row()
{
	if (selected_row == menu_choices.size())
	{
		if (highlight)
		{
			wattron(menu_win, A_STANDOUT);
		}
		mvwaddstr(menu_win, 10, my_menu.width / 2 - 4, "[Start]");
		wattroff(menu_win, A_STANDOUT);
		highlight = !highlight;
		return;
	}

	int num_choices = menu_choices[selected_row].size();
	mvwaddstr(menu_win, 4 + 2 * selected_row, 3, choice_labels[selected_row].c_str());
	waddch(menu_win, ' ');
	char input;
	for (int i = 0; i < num_choices; i++)
	{
		waddstr(menu_win, "  ");
		if (i == selected_choices[selected_row] && highlight)
		{
			wattron(menu_win, A_STANDOUT);
		}
		waddstr(menu_win, menu_choices[selected_row][i].c_str());
		wattroff(menu_win, A_STANDOUT);
	}
	highlight = !highlight;
}

void StartMenu::switch_selected_row(int row_change)
{
	if(selected_row == menu_choices.size())
	{
		highlight = false;
	}
	else
	{
		highlight = true;
	}

	update_selected_row();
	highlight = true;
	selected_row += row_change;
	if (selected_row < 0)
	{
		selected_row = 0;
	}
	else if(selected_row > menu_choices.size())
	{
		selected_row = menu_choices.size();
	}
}

void StartMenu::switch_selected_choice(int choice_change)
{
	selected_choices[selected_row] += choice_change;
	if(selected_choices[selected_row] < 0)
	{
		selected_choices[selected_row] = 0;
	}
	else if(selected_choices[selected_row] > menu_choices[selected_row].size() - 1)
	{
		selected_choices[selected_row] = menu_choices[selected_row].size() - 1;
	}
	highlight = true;
}

int StartMenu::handle_enter()
{
	if (selected_row < menu_choices.size())
	{
		switch_selected_row(1);
		return M_OK;
	}
	return M_RECREATE_ME;
}

Scene* StartMenu::create_new()
{
	std::string *time_str = &menu_choices[2][selected_choices[2]];
	Scene *new_scene = new Game(selected_choices[0], selected_choices[1], time_str);

	// DEBUG
	debug_out << "New Object Created" << std::endl;

	return new_scene;
}

StartMenu::~StartMenu()
{
	// DEBUG
	debug_out << "Destructing Derived Scene" << std::endl;

	werase(menu_win);
	wrefresh(menu_win);
	delwin(menu_win);
}