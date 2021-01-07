#pragma once

#include <GL/glew.h>
#include <string>

void init();
void loadTexture(GLuint& texture, const std::string& texturePath);
void update(struct GLFWwindow* window, float deltaTime);
void display();
void frameBufferResizeCallback(struct GLFWwindow* window, int width, int height);
void mouseCallback(struct GLFWwindow* window, double xPos, double yPos);
void processReceivedInput(struct GLFWwindow* window, float deltaTime);
float calcDeltaTime();
