#pragma once

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../../../../include/glad/glad.h" 

#include "StartUI.h"

/**
 * Not fully implemented yet, a window is created, but nothing is ever drawn.
*/
class StartGUI : public StartUI
{
private:
	virtual void update_selected_row();
	virtual void switch_selected_row(int row_change);
	virtual void switch_selected_choice(int choice_change);
	virtual int handle_enter();
	void draw();
	
	float vertices[18] = {
		0.0, 0.5, 0.0,
		0.5, -0.5, 0.0,
		-0.5, -0.5, 0.0
	};
	unsigned int indices[6] = {
		0, 1, 2,
	};

	GLuint VBO, VAO, EBO;
	unsigned int shader_program;

public:
	StartGUI();
	virtual int update();
	virtual void init_menu();
	virtual std::string *get_time_str();
	virtual ~StartGUI();

	int selected_choices[3] = {0, 0, 0};
};
