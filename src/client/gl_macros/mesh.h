#ifndef M_MESH
#define M_MESH

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			std::vector<Texture> tex_coords);
	void draw(Shader &shader);

private:
	unsigned int VAO, VBO, EBO;
	void setup_mesh();
};

#endif
