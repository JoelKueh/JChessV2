
#ifndef H_SCENE
#define H_SCENE

#include "../defs.h"

#include <ncurses.h>
#define ctrl(x) ((x) & 0x1f)

class scene
{
public:
	scene();
	virtual void init() = 0;
	virtual int update() = 0;
	virtual scene* get_next() = 0;
	virtual ~scene();

	enum state
	{
		M_ERR = -1,
		M_TERMINATE = 0,
		M_OK = 1,
		M_RECREATE_ME = 2
	};
};

#endif
