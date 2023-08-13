#include "StartGUI.h"

#include <filesystem>

extern GLFWwindow* window;
extern std::string exe_dir;
std::string resource_root;

extern Model *chess_set[2][6];
extern Model *board;

StartGUI *StartGUI::inst = nullptr;

StartGUI *StartGUI::get_inst()
{
	if (!inst)
		inst = new StartGUI();
	return inst;
}

void StartGUI::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	StartGUI *gui = StartGUI::get_inst();

	if (gui->first_mouse)
	{
		gui->last_x = xpos;
		gui->last_y = ypos;
		gui->first_mouse = false;
	}

	float xoffset = xpos - gui->last_x;
	float yoffset = ypos - gui->last_y;
	gui->last_x = xpos;
	gui->last_y = ypos;

	gui->camera->turn_cam(xoffset, -yoffset);
}

void StartGUI::scroll_callback(GLFWwindow *window,
		double xoffset, double yoffset)
{
	StartGUI *gui = StartGUI::get_inst();

	gui->camera->update_zoom(xoffset);
}

void StartGUI::window_focus_callback(GLFWwindow *window, int focused)
{
	StartGUI *gui = StartGUI::get_inst();
	if (!focused)
	{
		gui->first_mouse = true;
		gui->first_frame = true;
	}
}

StartGUI::StartGUI()
{
	resource_root = exe_dir + "resources/client/StartMenu/";

	shader = new Shader((resource_root + "vertex_shader.glsl").c_str(),
			(resource_root + "fragment_shader.glsl").c_str());
	camera = new Camera();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);

	piece_tex[0] = tex_from_file("chess_set/textures/white_albedo.jpg",
			resource_root.c_str(), false);
	piece_tex[1] = tex_from_file("chess_set/textures/black_albedo.jpg",
			resource_root.c_str(), false);
	board_tex = tex_from_file("chess_set/textures/doska_albedo.jpg",
			resource_root.c_str(), false);
}

int StartGUI::update()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 6.0f * deltaTime;

	UI_STATE state = U_OK;

	if (first_frame) {
		first_frame = false;
		return state;
	}

	float fvel = 0.0f, rvel = 0.0f, uvel = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		fvel = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		rvel = -1.0f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		fvel = -1.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rvel = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		uvel = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		uvel = -1.0f;
	camera->move_cam_relative(fvel, rvel, uvel, 1.0f, deltaTime);

	glfwPollEvents();
	draw();

	if (glfwWindowShouldClose(window))
		state = U_TERMINATE;

	first_frame = false;
	return state;
}

void StartGUI::draw()
{
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use();
	
	int width, height;
	glfwGetWindowSize(window, &width, &height);
        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom),
			(float)width / (float)height, 0.1f, 400.0f);
	glm::mat4 view = camera->get_view_matr();
        shader->set_mat4("projection", projection);
        shader->set_mat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_mat4("model", model);

	board->draw(*shader);
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 6; ++j) {
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        		shader->set_mat4("model", model);
			chess_set[i][j]->draw(*shader);
		}
	}
	
        glfwSwapBuffers(window);
        glfwPollEvents();
}

void StartGUI::init_menu()
{

}

std::string *StartGUI::get_time_str()
{
	return nullptr;
}

void StartGUI::update_selected_row()
{

}

void StartGUI::switch_selected_row(int row_change)
{

}

void StartGUI::switch_selected_choice(int choice_change)
{

}

int StartGUI::handle_enter()
{
	return 0;
}

StartGUI::~StartGUI()
{
	delete shader;
	delete camera;

	glDeleteTextures(1, &board_tex);
	glDeleteTextures(1, &piece_tex[0]);
	glDeleteTextures(1, &piece_tex[1]);

	inst = nullptr;
}
