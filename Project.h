#pragma once

#include <string>
#include <GL/glew.h>

void init();
void loadTexture(GLuint& texture, const std::string& texturePath);
void display();
