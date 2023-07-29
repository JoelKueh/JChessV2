#include "StartGUI.h"
#include <GLFW/glfw3.h>

extern GLFWwindow* window;
extern std::string exe_dir;

const char* vert_path = "resources/client/StartMenu/vertex_shader.glsl";
const char* frag_path = "resources/client/StartMenu/fragment_shader.glsl";

StartGUI::StartGUI()
{
	int success;
	char infoLog[512];

	// Compile the vertex shader
	std::string vert_shader_src;
	std::string vert_path_full = exe_dir + vert_path;
	read_file(vert_path_full.c_str(), vert_shader_src);
	const char* vert_shader_cstr = vert_shader_src.c_str();

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vert_shader_cstr, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog;
	}

	// Compile the fragment shader
	std::string frag_shader_src;
	std::string frag_path_full = exe_dir + frag_path;
	read_file(frag_path_full.c_str(), frag_shader_src);
	const char* frag_shader_cstr = frag_shader_src.c_str();
	
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &frag_shader_cstr, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAIED\n"
			<< infoLog;
	}

	// Link the vertex and fragment shaders together into
	// one shader program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
			<< infoLog;
	}

	// Delete the old shaders as they are already in the program
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Vertex Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind to the VAO we created and customize it
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
			indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
	glEnableVertexAttribArray(0);

	// DEBUG:
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void StartGUI::draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shader_program);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
}

int StartGUI::update()
{
	UI_STATE state = U_OK;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	draw();
	glfwPollEvents();

	if (glfwWindowShouldClose(window))
		state = U_TERMINATE;

	return state;
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);
}
