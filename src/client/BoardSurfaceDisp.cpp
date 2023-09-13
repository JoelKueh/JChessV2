#include "BoardSurfaceDisp.h"
#include "../common/CB_2_0/Utils.h"

// Debug
#include <iostream>

extern Shader *set_shdr;

BoardSurfaceDisplay::BoardSurfaceDisplay()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
			indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

	glBindVertexArray(0);

	std::cout << vertices[3].position.x << " | " << vertices[3].position.y;
}

void BoardSurfaceDisplay::draw_mask(uint64_t mask, const glm::vec3 &color)
	const 
{
	glBindVertexArray(VAO);
	while(mask) {
		int sq = CB::pop_rbit(mask);
		int row = sq / 8;
		int col = sq % 8;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model,
				glm::vec3(TL_OFFSET.x + col * SQUARE_WIDTH,
					0.0f,
					TL_OFFSET.y - row * SQUARE_WIDTH));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		set_shdr->set_mat4("model", model);
		set_shdr->set_vec3("color", color);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void BoardSurfaceDisplay::draw_base() const
{
	const uint64_t white_mask = 0x55AA55AA55AA55AA;
	const uint64_t black_mask = 0xAA55AA55AA55AA55;

	draw_mask(black_mask, glm::vec3(0.20f, 0.20f, 0.20f));
	draw_mask(white_mask, glm::vec3(0.95f, 0.95f, 0.95f));
}
