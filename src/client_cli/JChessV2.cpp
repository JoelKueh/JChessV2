
#include "defs.h"
#include "client.h"

#include <ncurses.h>
#include <iostream>
#include <unistd.h>

#ifdef FILE_DEBUG
#include <fstream>
std::ofstream debug_out;
#endif

int main(int argc, char *argv[])
{
#ifdef FILE_DEBUG
	debug_out.open("out/output.txt");
#endif
	
	client cli;
	while (cli.poll_set());
	std::cout << "Exiting..." << std::endl;
}
