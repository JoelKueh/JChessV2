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

class StartMenu : public Scene
{
private:
	StartUI *UI;

public:
	StartMenu();
	virtual void init();
	virtual int update();
	virtual Scene* create_new();
	virtual ~StartMenu();
};