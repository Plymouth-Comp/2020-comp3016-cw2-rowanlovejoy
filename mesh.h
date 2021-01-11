#pragma once

#include "shader.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 Position{};
	glm::vec3 Normal{};
	glm::vec2 TexCoords{};
	glm::vec3 Tangent{};
	glm::vec3 Bitangent{};
};

struct Texture
{
	unsigned int id{};
	std::string type{};
	std::string path{};
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

	void draw(const Shader& shader) const;

private:
	std::vector<Vertex> vertices{};
	std::vector<unsigned int> indices{};
	std::vector<Texture> textures{};
	unsigned int VAO{};

	unsigned int VBO{};
	unsigned int EBO{};

	void setupMesh();
};