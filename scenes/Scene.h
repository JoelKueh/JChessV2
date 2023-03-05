#pragma once

#include <ncurses.h>
#define ctrl(x)           ((x) & 0x1f)

class Scene
{
public:
	Scene();
  	virtual void init() = 0;
	virtual int update() = 0;
	virtual Scene* create_new() = 0;
  	virtual ~Scene();
	
	enum update_states
	{
    M_ERR = -1,
		M_TERMINATE = 0,
		M_OK = 1,
		M_RECREATE_ME = 2,
	};
};