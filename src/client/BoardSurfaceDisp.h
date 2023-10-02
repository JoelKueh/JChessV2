#ifndef BOARD_SURFACE_DISPLAY
#define BOARD_SURFACE_DISPLAY

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl_macros/shader.h"
#include "gl_macros/mesh.h"

class BoardSurfaceDisplay
{
public:
	BoardSurfaceDisplay();

	void draw_mask(uint64_t mask, const glm::vec3 &color) const;
	void draw_base() const;

	~BoardSurfaceDisplay() = default;
private:
	const float BOARD_WIDTH = 12.0f;
	const float SQUARE_WIDTH = BOARD_WIDTH / 8.0f;
	const glm::vec2 TL_OFFSET = { -6.0f + SQUARE_WIDTH / 2.0f,
		6.0f - SQUARE_WIDTH / 2.0f };

	const Vertex vertices[4] = {
		{ // Top Left
			glm::vec3(-SQUARE_WIDTH / 2.0f, 0.01f, SQUARE_WIDTH / 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},
		{ // Top Right
			glm::vec3(SQUARE_WIDTH / 2.0f, 0.01f,  SQUARE_WIDTH / 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},
		{ // Bottom Right
			glm::vec3(SQUARE_WIDTH / 2.0f, 0.01f, -SQUARE_WIDTH / 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},
		{ // Bottom Left
			glm::vec3(-SQUARE_WIDTH / 2.0f, 0.01f, -SQUARE_WIDTH / 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		}
	};
	const unsigned int indices[6] = {
		0, 2, 1,
		0, 3, 2
	};

	unsigned int VBO, VAO, EBO;
};

#endif
