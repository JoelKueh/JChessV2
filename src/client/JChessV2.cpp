#include <cstring>
#include <ncurses.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../../include/glad/glad.h"
#include "gl_macros/stb_image.h"
#include "gl_macros/shader.h"
#include "gl_macros/model.h"
#include "BoardSurfaceDisp.h"

#include "scenes/StartMenu/StartMenu.h"
#include "Global.h"

// DEBUG
// Tail the file output.txt in another terminal to get debug output from the fstream
#include <fstream>
std::stream debug_out;

Scene *my_scene;
options my_options;
std::string exe_dir;

// TERMINAL GLOBALS
struct winsize terminal;
int scr_x;
int scr_y;

// GUI GLOBALS
GLFWwindow* window;

Model *chess_set[2][6];
Model *board;
Shader *set_shdr;
BoardSurfaceDisplay *board_surface;
std::string piece_fname[2][6] = {
	{
		"Black_Pawn.dae",
		"Black_Knight.dae",
		"Black_Bishop.dae",
		"Black_Rook.dae",
		"Black_Queen.dae",
		"Black_King.dae"
	},
	{
		"White_Pawn.dae",
		"White_Knight.dae",
		"White_Bishop.dae",
		"White_Rook.dae",
		"White_Queen.dae",
		"White_King.dae"
	}
};

/**
 * Loops through all of the arguments passed into the program
 * @param argc Direct pass from main arguments
 * @param argv Also direct pass from main
*/
void parse_args(int argc, char **argv)
{
	// HACKY CODE TO MAKE SURE THAT RESOURCES ARE ALWAYS FOUND
	exe_dir = argv[0];
	while (exe_dir.back() != '/')
		exe_dir.pop_back();

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
	use_default_colors();
	start_color();

	// Get terminal size
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal);
	scr_x = terminal.ws_col;
	scr_y = terminal.ws_row;

	return 1;
}

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

/**
 * To be run if the gui argument was passed into program execution
*/
int init_gui()
{
	// Init gui, if fails, quit.
	if (!glfwInit()) {
		std::cerr << "GLFW INITIALIZATION FAILED" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(glfw_error_callback);

	// Create a window
	window = glfwCreateWindow(800, 600, "JChessV2", NULL, NULL);
	if (!window)
	{
		std::cerr << "GLFW Window Creation Failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Draw to window
	glfwMakeContextCurrent(window);

	// Get pointers to OpenGL driver functions at runtime.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the viewport
	glViewport(0, 0, 800, 600); 
	stbi_set_flip_vertically_on_load(false);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glfwSwapBuffers(window);

	// Load Chess Board Models
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 6; ++j) {
			std::string path = exe_dir
				+ "resources/client/chess_set/Models/"
				+ piece_fname[i][j];
			chess_set[i][j] = new Model(path.c_str());
		}
	}
	board = new Model((exe_dir
			+ "resources/client/chess_set/"
			+ "Models/Board.dae").c_str());
	set_shdr = new Shader((exe_dir + "resources/client/chess_set/"
			+ "Shaders/set_shdr.vert").c_str(),
			(exe_dir + "resources/client/chess_set/"
			+ "Shaders/set_shdr.frag").c_str());
	board_surface = new BoardSurfaceDisplay;

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
	// Destroy Models
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 6; ++j) {
			delete chess_set[i][j];
		}
	};
	delete board;
	delete set_shdr;
	delete board_surface;

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
