
#include "start_menu.h"

#ifdef FILE_DEBUG
#include <fstream>
extern std::ofstream debug_out;
#endif

start_menu::start_menu()
{

}

void start_menu::init()
{

}

int start_menu::update()
{

}

scene *start_menu::get_next()
{

}

void update_row()
{

}

void switch_row(int change)
{

}

void switch_choice(int change)
{

}

int handle_enter()
{

}

void draw()
{

}

void init_menu()
{

}

std::string *get_time_str()
{

}

start_menu::~start_menu()
{
#if defined(FILE_DEBUG) & defined(DEBUG_VERBOSE)
	debug_out << "Destroying Derived Scene" << std::endl;
#endif
}
