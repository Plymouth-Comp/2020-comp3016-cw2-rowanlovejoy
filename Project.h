#pragma once

#include <GL/glew.h>
#include <string>

void init();
void loadTexture(GLuint& texture, const std::string& texturePath);
void update();
void display();
void frameBufferResizeCallback(struct GLFWwindow* window, int width, int height);
void processReceivedInput(struct GLFWwindow* window);