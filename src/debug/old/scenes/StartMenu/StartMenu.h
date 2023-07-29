#pragma once

#include "../Game/Game.h"
#include "../../Global.h"
#include "StartCLI.h"
#include "StartGUI.h"
#include "StartUI.h"
#include "../Scene.h"

#include <string>
#include <vector>

extern options my_options;

/**
 * This class is a derivation of scene. This allows for the main function to
 * call the update loop on any scene and not worry about the varyinig
 * functionality of the contents of that class. This class does not have much 
 * content because almost all of the functionality of the menu system is
 * handled in the UI, this is however, not the case for the Game scene.
*/
class StartMenu : public Scene
{
private:
	StartUI *UI;

public:
	/**
	 * Constructs a Start Menu scene.
	 * Creates a StartUI object which can either be a GUI or a CLI.
	*/
	StartMenu();

	/**
	 * Handles initilization of the start menu
	*/
	virtual void init();

	/**
	 * The main loop of the start menu, called once every cycle
	*/
	virtual int update();

	/**
	 * Creates the successor to this scene.
	*/
	virtual Scene* create_new();

	/**
	 * Destroys the StartMenu object.
	*/
	virtual ~StartMenu();
};
