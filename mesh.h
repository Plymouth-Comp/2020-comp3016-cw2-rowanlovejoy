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
	unsigned int Id{};
	std::string Type{};
	std::string Path{};
};

// Class representing a 3D mesh that can be rendered
class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

	void draw(const Shader& shader, const glm::mat4& transform) const;

private:
	std::vector<Vertex> Vertices{};
	std::vector<unsigned int> Indices{};
	std::vector<Texture> Textures{};
	unsigned int VAO{};

	unsigned int VBO{};
	unsigned int EBO{};

	void setUpMesh();
};