#pragma once

#include "gameobject.h"

class VisibleObject : public GameObject
{
public:
	VisibleObject(const glm::vec3& pos, const glm::vec3& siz, const Model& mod);

	virtual void draw(const Shader& shader) const override;

private:
	Model ObjectModel;
};
