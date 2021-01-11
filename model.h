#pragma once

#include "mesh.h"
#include "shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <vector>

class Model
{
public:
	// constructor, expects a filepath to a 3D model.
	Model(const std::string& path, bool gamma = false);

	// draws the model, and thus all its meshes
	void draw(const Shader& shader) const;

private:
	// model data 
	std::vector<Texture> texturesLoaded{};	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes{};
	std::string directory{};
	bool gammaCorrection{};

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadSceneFromFile(const std::string& path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void getMeshesInNode(const aiNode* node, const aiScene* scene);

	Mesh storeMeshData(const aiMesh* mesh, const aiScene* scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> storeMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);

	static unsigned int loadTextureFromFile(const std::string& path, const std::string& directory, bool gamma = false);
};
