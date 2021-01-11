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
	Model(const std::string& path, bool gamma = false);

	void draw(const Shader& shader) const;

private:
	std::vector<Texture> texturesLoaded{};
	std::vector<Mesh> meshes{};
	std::string directory{};
	bool gammaCorrection{};

	void loadSceneFromFile(const std::string& path);

	void getMeshesInNode(const aiNode* node, const aiScene* scene);

	Mesh storeMeshData(const aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> storeMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);

	static unsigned int loadTextureFromFile(const std::string& path, const std::string& directory, bool gamma = false);
};
