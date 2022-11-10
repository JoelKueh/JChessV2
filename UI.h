#pragma once

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>
#include <wchar.h>

//wchar_t UNI_HZ_LN = '\u2500';
//const U_vr_ln = 0x2502;
//const UChar d_r = 0x250c;
//const UChar d_lr = 0x252c;
//const UChar d_l = 0x2510;
//const UChar du_r = 0x251c;
//const UChar du_lr = 0x254c;
//const UChar du_l = 0x2524;
//const UChar u_r = 0x2514;
//const UChar u_lr = 0x2534;
//const UChar u_l = 0x2518;

class UI
{
public:
	Graphics();
	void draw_chess_board(int start_x, int start_y);
	void cleanup();
	void wait_key_press();

private:
	void draw_piece_line();
	void draw_other_line();
};

class UINoGUI : public UI
{
public:
	void draw_chess_board(int start_x, int start_y);
private:
	void draw_top_line();
	void draw_piece_line();
	void draw_other_line();
	void draw_bottom_line();
}
