#include "UI.h"

Graphics myGraphics;

int main()
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	myGraphics.draw_chess_board(0,0);

	myGraphics.wait_key_press();
	myGraphics.cleanup();
	return 0;
}

