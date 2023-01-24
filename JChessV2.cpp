#include <cstring>
#include <ncurses.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <GLFW/glfw3.h>

#include "scenes/StartMenu/StartMenu.h"
#include "Global.h"

// DEBUG
#include <fstream>
std::ofstream debug_out;

Scene *my_scene;

options my_options;

// TERMINAL GLOBALS
struct winsize terminal;
int scr_x;
int scr_y;

// GUI GLOBALS
GLFWwindow* window;

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

int init_terminal()
{
	setlocale(LC_CTYPE, "");
	initscr();
	
	noecho();
	timeout(500);
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);
	scr_x = terminal.ws_col;
	scr_y = terminal.ws_row;

	return 1;
}

int init_gui()
{
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	return 1;
}

int init()
{
	if(my_options.gui)
		return init_gui();

	return init_terminal();
}

int cleanup_terminal()
{
	endwin();
	return 1;
}

int cleanup_gui()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	return 1;
}

int cleanup()
{
	if(my_options.gui)
		return cleanup_gui();
		
	return cleanup_terminal();
}

void remake_scene()
{
	Scene* new_scene = my_scene->create_new();
	delete my_scene;
	my_scene = new_scene;
	my_scene->init();
}

int main(int argc, char *argv[])
{
	// DEBUG
	debug_out.open("out/output.txt");

	parse_args(argc, argv);

	init();

	my_scene = new StartMenu();
	my_scene->init();

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

	cleanup();

	return 0;
}