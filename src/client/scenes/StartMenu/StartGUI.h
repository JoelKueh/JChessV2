#ifndef H_START_GUI
#define H_START_GUI

#include "StartUI.h"

// Window manager and OpenGL function loader
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../../../../include/glad/glad.h" 

// Helpful utils
#include "../../gl_macros/gl_macros.h"
#include "../../gl_macros/shader.h"
#include "../../gl_macros/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Implement function binding.
// When creating callbacks for OpenGL, I want to specify which object should
// be edited when the callback is run. This allows me to create a new function
// from one of my member functions where "this" is locked in to a particular
// object. Passing it in as a callback will then edit the right variables.
//
// This actually isn't that useful as the single window limitation limits
// us to one GUI anyway, but I'm just screwing around at this point
// 
// Theoretically, one could pass a window as an input to this class and allow
// two GUI's to be run at once, but that would require passing initialization
// data in as a void* because this is virtual, and I don't feel like doing
// that at the moment.
#include <binders.h>

class StartGUI : public StartUI
{
private:
	virtual void update_selected_row();
	virtual void switch_selected_row(int row_change);
	virtual void switch_selected_choice(int choice_change);
	virtual int handle_enter();
	void draw();

public:
	StartGUI();
	virtual int update();
	virtual void init_menu();
	virtual std::string *get_time_str();
	virtual ~StartGUI();

	int selected_choices[3] = {0, 0, 0};
};

#endif
