#include "visibleobject.h"
#include <glm/ext/matrix_transform.hpp>

VisibleObject::VisibleObject(const glm::vec3& pos, const glm::vec3& siz, const Model& mod) : GameObject{pos, siz}, ObjectModel{mod} { }

void VisibleObject::draw(const Shader& shader) const
{
	auto transform{glm::mat4{1.0f}};
	transform = glm::translate(transform, Position);

	ObjectModel.draw(shader, transform);
}
