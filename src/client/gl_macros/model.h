#ifndef M_MODEL
#define M_MODEL

#include "shader.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const char *path)
	{
		load_model(path);
	}
	void draw(Shader &shader)
	{
		for (unsigned int i = 0; i < meshes.size(); ++i) {
			meshes[i].draw(shader);
		}
	}


private:
	std::vector<Mesh> meshes;
	std::string directory;

	void load_model(std::string path);
	void process_node(const aiNode *node, const aiScene *scene);
	Mesh process_mesh(const aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> load_material_texture(aiMaterial *mat,
			aiTextureType type, std::string type_name);
};

unsigned int tex_from_file(const char *path,
		const std::string &directory, bool gama);

#endif
