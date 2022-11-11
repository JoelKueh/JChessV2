#pragma once

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>
#include <wchar.h>

typedef uint32_t UChar;
const UChar hz_ln = 0x2500;
const UChar vr_ln = 0x2502;
const UChar d_r = 0x250c;
const UChar d_lr = 0x252c;
const UChar d_l = 0x2510;
const UChar du_r = 0x251c;
const UChar du_lr = 0x253c;
const UChar du_l = 0x2524;
const UChar u_r = 0x2514;
const UChar u_lr = 0x2534;
const UChar u_l = 0x2518;

class UI
{
public:
	UI();
	virtual void draw_chess_board(int start_x, int start_y) = 0;
	void cleanup();
	void wait_key_press();
};

class UnicodeUI : public UI
{
public:
	UnicodeUI();
	void draw_chess_board(int start_x, int start_y);

private:
	void draw_top_line();
	void draw_piece_line();
	void draw_other_line();
	void draw_bottom_line();
};

class NoUnicodeUI : public UI
{
public:
	NoUnicodeUI();
	void draw_chess_board(int start_x, int start_y);

private:
	void draw_piece_line();
	void draw_other_line();
};
