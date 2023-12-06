
#ifndef H_START_MENU
#define H_START_MENU

#include "../../defs.h"
#include "../scene.h"

#include <string>
#include <vector>
#include <ncurses.h>

class start_menu : public scene
{
public:
	start_menu();
	virtual void init();
	virtual int update();
	virtual scene *get_next();
	virtual ~start_menu();

private:
	WINDOW *menu_win;
	bool highlight = false;

	void update_row();
	void switch_row(int change);
	void switch_choice(int change);
	int handle_enter();

	void draw();
	void init_menu();
	std::string *get_time_str();

	int selected_choices[3] = {0, 0, 0};
};

#endif
