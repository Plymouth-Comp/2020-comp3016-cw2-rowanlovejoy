#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

void init();
GLuint loadTexture(const std::string& path);
void update(GLFWwindow* window, float deltaTime);
void frameBufferResizeCallback(struct GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void processReceivedInput(GLFWwindow* window, float deltaTime);
float calcDeltaTime();
