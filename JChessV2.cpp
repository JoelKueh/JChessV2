#include <cstring>
#include <ncurses.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <GLFW/glfw3.h>

#include "scenes/StartMenu/StartMenu.h"
#include "Global.h"

// DEBUG
// Tail the file output.txt in another terminal to get debug output from the fstream
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

/**
 * Loops through all of the arguments passed into the program
 * @param argc Direct pass from main arguments
 * @param argv Also direct pass from main
*/
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

/**
 * To be run if the program was executed in GUI mode
*/
int init_terminal()
{
	// Set char type
	setlocale(LC_CTYPE, "");
	initscr();
	
	// Set terminal settings
	noecho();
	timeout(500);
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);

	// Get terminal size
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);
	scr_x = terminal.ws_col;
	scr_y = terminal.ws_row;

	return 1;
}

/**
 * To be run if the gui argument was passed into program execution
*/
int init_gui()
{
	// Init gui, if fails, quit.
	if (!glfwInit())
		return -1;

	// Create a window
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Draw to window
	glfwMakeContextCurrent(window);

	return 1;
}

/**
 * Reads the global my_options struct and calls the necessary init method
*/
int init()
{
	if(my_options.gui)
		return init_gui();

	return init_terminal();
}

/**
 * To be run at the end of the program in CLI mode.
 * Get the terminal back in the proper state.
 * If cleanup never runs, either reset or (maybe) tty sane needs to be run.
*/
int cleanup_terminal()
{
	endwin();
	return 1;
}

/**
 * To be run at the end of the program in GUI mode.
 * Ends the window that was created with the program.
*/
int cleanup_gui()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	return 1;
}

/**
 * Calls all of the necessary cleanup functions on program termination.
*/
int cleanup()
{
	if(my_options.gui)
		return cleanup_gui();
		
	return cleanup_terminal();
}

/**
 * Handles transitioning between scenes.
*/
void remake_scene()
{
	// Current scene makes its successor.
	Scene* new_scene = my_scene->create_new();

	// Current scene is destroyed.
	delete my_scene;

	// New scene is set to be used and initialized.
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

	// Much of the main loop is controlled by whatever scene is currently active.
	// This sends an update call to whatever scene is currently running, and uses the response from
	// that update call to decide what to do.
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