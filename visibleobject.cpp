#include "visibleobject.h"
#include <glm/ext/matrix_transform.hpp>

VisibleObject::VisibleObject(const glm::vec3& pos, const glm::vec3& siz, const Model& mod, const glm::vec3& scale, const glm::vec3& offset) : GameObject{pos, siz}, ObjectModel{mod}, Scale{scale}, Offset{offset} { }

void VisibleObject::draw(const Shader& shader) const
{
	auto transform{glm::mat4{1.0f}};

	transform = glm::translate(transform, Position);
	
	// Offset models slightly on X and Y to line up with collision box
	transform = glm::translate(transform, Offset);

	transform = glm::scale(transform, Scale);

	ObjectModel.draw(shader, transform);
}
