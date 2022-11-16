#include <cstring>

#include "head/scenes/MainMenu.h"

Scene *my_scene;

struct options
{
	bool uni = true;
	bool gui = false;
};
options my_options;

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

int main(int argc, char *argv[])
{
	parse_args(argc, argv);

	setlocale(LC_CTYPE, "");
	initscr();

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	my_scene = new(MainMenu());

	return 0;
}
