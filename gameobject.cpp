#include "gameobject.h"
#include <glm/ext/matrix_transform.hpp>

GameObject::GameObject(const glm::vec3& pos, const glm::vec3& siz, const glm::vec3& vel) : Position{pos}, Size{siz}, Velocity{vel} {}

void GameObject::draw(const Shader& shader) const { }
