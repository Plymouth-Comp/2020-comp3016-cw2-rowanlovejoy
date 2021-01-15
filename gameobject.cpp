#include "gameobject.h"
#include <glm/ext/matrix_transform.hpp>

GameObject::GameObject(const glm::vec3& pos, const glm::vec3& siz, const glm::vec3& vel) : Position{pos}, Size{siz}, Velocity{vel} {}

void GameObject::draw(const Shader& shader) const { }

void GameObject::setPosition(const glm::vec3& newPos)
{
	// Update position
	Position = newPos;
}

void GameObject::move(float deltaTime)
{
	Position += Velocity * deltaTime;

	// Consume velocity
	Velocity = glm::vec3{0.0f};
}

void GameObject::addMovementInput(const glm::vec3& direction)
{
	Velocity += direction;
}
