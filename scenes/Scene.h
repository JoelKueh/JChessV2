#pragma once

#include <ncurses.h>
#define ctrl(x)           ((x) & 0x1f)

class Scene
{
public:
	Scene();
	virtual int update() = 0;
	virtual Scene* create_new() = 0;
	
	enum update_states
	{
		M_TERMINATE = 0,
		M_OK = 1,
		M_RECREATE_ME = 2,
	};

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
};