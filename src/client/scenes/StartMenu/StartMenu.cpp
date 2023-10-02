#include "StartMenu.h"
#include "../../Global.h"

// DEBUG
#include <fstream>
extern std::ofstream debug_out;

StartMenu::StartMenu()
{
	if (my_options.gui)
	{
		UI = StartGUI::get_inst();
	}
	else
	{
		UI = StartCLI::get_inst();
	}
}

void StartMenu::init()
{
	UI->init_menu();
}

int StartMenu::update()
{
	return UI->update();
}

Scene* StartMenu::create_new()
{
	std::string *time_str = UI->get_time_str();
	Scene *new_scene = new Game(UI->selected_choices[0], UI->selected_choices[1], time_str);

	// DEBUG
	debug_out << "New Object Created" << std::endl;

	return new_scene;
}

StartMenu::~StartMenu()
{
	// DEBUG
	debug_out << "Destructing Derived Scene" << std::endl;
	delete UI;
}
