#include <cstring>

#include "scenes/StartMenu/StartMenu.h"
#include <ncurses.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

Scene *my_scene;

struct options
{
	bool uni = true;
	bool gui = false;
};
options my_options;

struct winsize terminal;

void parse_args(int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		if(strcmp(*(argv + i), "nouni") == 0)
		{
			my_options.uni = false;
		}
		else if (strcmp(*(argv + i), "gui") == 0)
		{
			my_options.gui = true;
		}
	}
}

void init_terminal()
{
	setlocale(LC_CTYPE, "");
	initscr();
	noecho();
	timeout(700);
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);
}

void cleanup()
{
	endwin();
}

void remake_scene()
{
	Scene* new_scene = my_scene->create_new();
	delete my_scene;
	my_scene = new_scene;
}

int main(int argc, char *argv[])
{
	parse_args(argc, argv);
	init_terminal();

	my_scene = new StartMenu(terminal.ws_col, terminal.ws_row);
	// DEBUG
	// printw("Width: %d\nHeight: %d",terminal.ws_col, terminal.ws_row);

	bool quit = false;
	while (!quit)
	{
		int update_state = 0;
		update_state = my_scene->update();

		switch (update_state)
		{
			case Scene::update_states::M_TERMINATE: quit = true; break;
			case Scene::update_states::M_RECREATE_ME: remake_scene(); break;
			case Scene::update_states::M_OK: break;
			default: break;
		}
	}
	wgetch(stdscr);
	cleanup();

	return 0;
}