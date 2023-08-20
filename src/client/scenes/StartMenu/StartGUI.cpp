#include "StartGUI.h"

#include <filesystem>

extern GLFWwindow* window;
extern std::string exe_dir;
std::string resource_root;

extern Model *chess_set[2][6];
extern Model *board;
extern Shader *board_shdr;
extern Shader *piece_shdr;

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

	piece_shdr->use();

	char board_char[8][8];
	board_pieces.board_to_str(board_char);
	
	int width, height;
	glfwGetWindowSize(window, &width, &height);
        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom),
			(float)width / (float)height, 0.1f, 400.0f);
	glm::mat4 view = camera->get_view_matr();
        piece_shdr->set_mat4("projection", projection);
        piece_shdr->set_mat4("view", view);

        glm::mat4 init = glm::mat4(1.0f);
	glm::mat4 model;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			const float width = 1.5f;
			float x = col * width - 5.25f;
			float y = row * width - 5.25f;
			model = glm::translate(init, glm::vec3(x, 0.0f, y));
			model = glm::rotate(model, glm::radians(180.0f),
					glm::vec3(0.0f, 1.0f, 0.0f));
			piece_shdr->set_mat4("model", model);
			draw_piece(board_char[row][col]);
		}
	}

	board_shdr->use();
	board_shdr->set_mat4("projection", projection);
	board_shdr->set_mat4("view", view);
	model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	board_shdr->set_mat4("model", model);
	board->draw(*board_shdr);
	
        glfwSwapBuffers(window);
        glfwPollEvents();
}

void StartGUI::draw_piece(char piece)
{
	Model *model;
	bool is_white;

	switch (piece) {
		case 'P': is_white = true; model = chess_set[1][0]; break;
		case 'N': is_white = true; model = chess_set[1][1]; break;
		case 'B': is_white = true; model = chess_set[1][2]; break;
		case 'R': is_white = true; model = chess_set[1][3]; break;
		case 'Q': is_white = true; model = chess_set[1][4]; break;
		case 'K': is_white = true; model = chess_set[1][5]; break;
		case 'p': is_white = false; model = chess_set[0][0]; break;
		case 'n': is_white = false; model = chess_set[0][1]; break;
		case 'b': is_white = false; model = chess_set[0][2]; break;
		case 'r': is_white = false; model = chess_set[0][3]; break;
		case 'q': is_white = false; model = chess_set[0][4]; break;
		case 'k': is_white = false; model = chess_set[0][5]; break;
		default: return;
	}

	float color = is_white ? 0.95 : 0.35;

	piece_shdr->set_float("color", color);
	model->draw(*piece_shdr);
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

	inst = nullptr;
}
