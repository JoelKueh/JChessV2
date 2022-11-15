#include <cstring>

#include "UI.h"
#include "ChessBoard.h"

UI* my_ui;

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

	if(my_options.uni)
	{
		my_ui = new(UnicodeUI);
	}
	else
	{
		my_ui = new(NoUnicodeUI);
	}

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	my_ui->wait_key_press();
	my_ui->cleanup();
	return 0;
}
