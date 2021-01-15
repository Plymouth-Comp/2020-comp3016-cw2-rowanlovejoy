#pragma once

#include "mesh.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>
#include <fstream>
#include <vector>

class Model
{
public:
	Model(const std::string& path);

	void draw(const Shader& shader, const glm::mat4& trans) const;

private:
	std::vector<Texture> TexturesLoaded{};
	std::vector<Mesh> Meshes{};
	std::string Directory{};

	void loadSceneFromFile(const std::string& path);

	void getMeshesInNode(const aiNode* node, const aiScene* scene);

	Mesh storeMeshData(const aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> storeMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);

	static unsigned int loadTextureFromFile(const std::string& path, const std::string& directory, bool gamma = false);
};
