#pragma once

#include "model.h"
#include "shader.h"
#include <glm/vec3.hpp>

class GameObject
{
public:
	virtual ~GameObject() = default;
	GameObject(const glm::vec3& pos, const glm::vec3& siz, const glm::vec3& vel = glm::vec3{0.0f});
	virtual void draw(const Shader& shader) const;
	
	void setPosition(const glm::vec3& newPos)
	{
		Position = newPos;
	}
	
	const glm::vec3& getPosition() const
	{
		return Position;
	}

	const glm::vec3& getSize() const
	{
		return Size;
	}
protected:
	glm::vec3 Position;
	glm::vec3 Size;
};
