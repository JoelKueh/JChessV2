
#include "defs.h"

#include <ncurses.h>
#include <iostream>

#ifdef FILE_DEBUG
#include <fstream>
std::ofstream debug_out;
#endif

int main(int argc, char *argv[])
{
#ifdef FILE_DEBUG
	debug_out.open("out/output.txt");
#endif
}
