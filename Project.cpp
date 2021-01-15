//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "Project.h"
#include "game.h"
#include "gameobject.h"
#include "model.h"
#include "shader.h"
#include "Character.h"
#include "stb_image.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

enum VAO_Ids
{
	objFirst, objSecond
};
enum VBO_Ids
{
	Vertices, Indices, Colours, Tex
};
enum Attrib_Ids
{
	posVertex = 0, posNormal = 1, posTexCoord = 2
};
enum Text_Ids
{
	textFirst
};

constexpr auto NUM_VAOs{2};
constexpr auto NUM_VBOs{1};

constexpr auto NUM_VERTICES{36};
constexpr auto NUM_TEXTURES{1};

constexpr auto SCREEN_WIDTH{800};
constexpr auto SCREEN_HEIGHT{600};

GLuint VAOs[NUM_VAOs]{};
GLuint VBOs[NUM_VBOs]{};
GLuint textures[NUM_TEXTURES]{};

Game gameInstance{SCREEN_WIDTH, SCREEN_HEIGHT};

void init()
{
	// Contains vertex positions and normals
	constexpr GLfloat vertices[]
	{
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	// Buffer initialisation
	glGenVertexArrays(NUM_VAOs, VAOs);
	glBindVertexArray(VAOs[objFirst]);

	glGenBuffers(NUM_VBOs, VBOs);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3 times float for position + 3 times float for normal
	constexpr auto attribStride{8 * sizeof(GLfloat)};

	// COLOUR CUBE
	// Vertex position attribute pointer
	glVertexAttribPointer(posVertex, 3, GL_FLOAT, GL_FALSE, attribStride, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(posVertex);
	// Vertex normal attribute pointer
	glVertexAttribPointer(posNormal, 3, GL_FLOAT, GL_FALSE, attribStride, BUFFER_OFFSET(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(posNormal);
	// Vertex texture coordinate attribute pointer
	glVertexAttribPointer(posTexCoord, 2, GL_FLOAT, GL_FALSE, attribStride, BUFFER_OFFSET(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(posTexCoord);

	// END COLOUR CUBE

	// LIGHT SOURCE
	glBindVertexArray(VAOs[objSecond]);
	// Reusing vertex data from colour cube, so just bind colour cube's VBO -- more efficient to reuse than spend cycles storing more in GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[Vertices]);
	// Vertex position attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribStride, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	// END LIGHT SOURCE
}

GLuint loadTexture(const std::string& path)
{
	GLuint textureId{};
	glGenTextures(1, &textureId);

	GLint width{};
	GLint height{};
	GLint nrComponents{};

	const auto data{stbi_load(path.c_str(), &width, &height, &nrComponents, 0)};

	if (data)
	{
		GLenum format{};
		switch (nrComponents)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			throw std::exception{"Error loading texture: invalid number of components\n"};
		}

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		const auto errorMsg{"Texture failed to load at path" + path + "\n"};
		throw std::exception{errorMsg.c_str()};
	}

	stbi_image_free(data);

	return textureId;
}

void update(GLFWwindow* window, float deltaTime)
{
	//// Adding all matrices up to create combined matrix
	//const auto mvp{projection * view * model};

	////adding the Uniform to the shader
	//shaderProgram->setUniform("mvp", mvp);
}

// Resize viewport on window resize
void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Callback on mouse cursor input
void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	static auto firstMouse{true};
	static auto lastX{static_cast<float>(SCREEN_WIDTH) / 2.0f};
	static auto lastY{static_cast<float>(SCREEN_HEIGHT) / 2.0f};

	// Handle first mouse input upon capture
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;

		firstMouse = false;
	}

	// Calculate mouse offset since last frame
	const auto xOffset{xPos - lastX};
	const auto yOffset{lastY - yPos}; // Y coordinates go from bottom top so operands are reverse from xOffset
	lastX = xPos;
	lastY = yPos;

	gameInstance.setMouseInput(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	gameInstance.setScrollInput(xOffset, yOffset);
}

// Process latest input received
void keyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	// Close window when escape is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			gameInstance.setKeyState(key, true);
		else if (action == GLFW_RELEASE)
			gameInstance.setKeyState(key, false);
	}
	
	//// Camera movement
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::FORWARD, g_deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::BACKWARD, g_deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::LEFT, g_deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::RIGHT, g_deltaTime);
}

void processInput(GLFWwindow* window, float deltaTime)
{
	//// Camera movement
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//	camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}

// Calculate time since last frame -- the delta
float calcDeltaTime()
{
	static auto timeLast{0.0f};

	const auto timeNow{static_cast<float>(glfwGetTime())};

	const auto deltaTime{timeNow - timeLast};

	timeLast = timeNow;

	return deltaTime;
}

//----------------------------------------------------------------------------
//
// main
//
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	const auto window{glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textured Cube", nullptr, nullptr)};
	if (!window)
	{
		std::cout << "Failed to create GLFW window.\n";

		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);

	// Capture and hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Register callback to call on mouse input
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	// Register callback to call on keyboard input
	glfwSetKeyCallback(window, keyInputCallback);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	// Viewport
	glewInit();

	//init();

	stbi_set_flip_vertically_on_load(true);

	// OpenGL config
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	gameInstance.init();

	//Shader basicShader{"shaders/shader.vert", "shaders/shader.frag"};
	//Model platformModel{"media/platform/platform.obj"};
	//GameObject platformObject{platformModel, glm::vec3{0.0f}};

	//const auto projection{glm::perspective(glm::radians(camera.getFov()), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f)};
	////basicShader.use();
	//basicShader.setUniform("projection", projection);
	
	while (!glfwWindowShouldClose(window))
	{
		const auto deltaTime{calcDeltaTime()};

		glfwPollEvents();

		//processInput(window, deltaTime);

		gameInstance.processInput();

		gameInstance.update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// View matrix
		//const auto view{camera.getViewMatrix()};
		//basicShader.setUniform("view", view);

		//platformObject.draw(basicShader);

		gameInstance.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
