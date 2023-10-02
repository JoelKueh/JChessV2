#pragma once

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../../../../include/glad/glad.h" 

#include "StartUI.h"
#include "../../gl_macros/gl_macros.h"
#include "../../gl_macros/shader.h"
#include "../../gl_macros/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class StartGUI : public StartUI
{
private:
	virtual void update_selected_row();
	virtual void switch_selected_row(int row_change);
	virtual void switch_selected_choice(int choice_change);
	virtual int handle_enter();
	void draw();
	
	/*float vertices[32] = {
		0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // TR
		0.5f,-0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // BR
	       -0.5f,-0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // BL
	       -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // TL 
	};*/
	float vertices[180] = {
	       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	       -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int indices[6] = {
		0, 1, 3,
		1, 2, 3
	};
	glm::vec3 cubePositions[10] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	GLuint VBO, VAO, EBO;
	unsigned int texture, texture_2;

	Shader *default_shader;

	float deltaTime, lastFrame;

public:
	StartGUI();
	virtual int update();
	virtual void init_menu();
	virtual std::string *get_time_str();
	virtual ~StartGUI();

	int selected_choices[3] = {0, 0, 0};
};
