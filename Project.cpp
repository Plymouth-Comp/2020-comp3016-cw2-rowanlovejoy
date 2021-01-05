//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "Project.h"
#include "stb_image.h"
#include "LoadShaders.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

enum VAO_Ids{objFirst};
enum VBO_Ids{Vertices, Indices, Colours, Tex};
enum Attrib_Ids{posVertex = 0, posTexCoord = 1};
enum Text_Ids{textFirst};

constexpr auto numVAOs{1};
constexpr auto numVBOs{4};

constexpr auto numVertices{36};
constexpr auto attribStride{5};
constexpr auto numTextures{1};

constexpr auto screenWidth{800};
constexpr auto screenHeight{600};

GLuint VAOs[numVAOs]{};
GLuint VBOs[numVBOs]{};
GLuint textures[numTextures]{};

// Shader program -- find a way for this to not be global
GLuint shaderProgram{};

//----------------------------------------------------------------------------
//
// init
//
void init()
{
	// GLEW provides functionality to load shaders from files
	ShaderInfo shaders[]
	{
		{GL_VERTEX_SHADER, "media/shader.vert"},
		{GL_FRAGMENT_SHADER, "media/shader.frag"},
		{GL_NONE, nullptr}
	};

	shaderProgram = LoadShaders(shaders);
	glUseProgram(shaderProgram);

	constexpr GLfloat vertices[]{
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	constexpr GLuint indices[][3]
	{  // note that we start from 0!
		{0, 3, 1},  // first Triangle front
		{3, 2, 1},   // second Triangle
		{4, 7, 0},
		{7, 3, 0},
		{1, 2, 5},
		{2, 6, 5},
		{5, 4, 0},
		{0, 1, 5},
		{2, 3, 7},
		{7, 6, 2},
		{4, 5, 7},  // first Triangle back
		{7, 5, 6}   // second Triangle
	};

	// Buffer initialisation
	glGenVertexArrays(numVAOs, VAOs);
	glBindVertexArray(VAOs[objFirst]);
	
	glGenBuffers(numVBOs, VBOs);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex position attribute pointers
	glVertexAttribPointer(posVertex, 3, GL_FLOAT, GL_FALSE, attribStride * sizeof(GLfloat), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(posVertex);

	// Texture coordinate attribute pointer
	glVertexAttribPointer(posTexCoord, 2, GL_FLOAT, GL_FALSE, attribStride * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(posTexCoord);

	loadTexture(textures[textFirst], "media/textures/awesomeface.png");
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
}

void loadTexture(GLuint& texture, const std::string& texturePath)
{
	// load and create a texture 
	// -------------------------

	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// load image, create texture and generate mipmaps
	GLint width{};
	GLint height{};
	GLint nrChannels{};
	
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	const auto textureData{stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0)};
	if (!textureData)
		std::cout << "Failed to load texture\n";

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(textureData);
}

void update()
{
	// creating the model matrix
	auto model{glm::mat4{1.0f}};
	//model = glm::scale(model, glm::vec3{2.0f, 2.0f, 2.0f});
	model = glm::rotate(model, glm::radians(50.0f) * static_cast<float>(glfwGetTime()), glm::vec3{0.5f, 1.0f, 0.0f});
	//model = glm::translate(model, glm::vec3{0.0f, 0.0f, -1.0f});

	// creating the view matrix
	auto view{glm::mat4{1.0f}};
	view = glm::translate(view, glm::vec3{0.0f, 0.0f, -3.0f});

	// creating the projection matrix
	const auto projection{glm::perspective(45.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f)};

	// Adding all matrices up to create combined matrix
	const auto mvp{projection * view * model};

	//adding the Uniform to the shader
	const auto mvpLoc{glGetUniformLocation(shaderProgram, "mvp")};
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
}

//----------------------------------------------------------------------------
//
// display
//
void display()
{
	glClearColor(0.0f, 0.25f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// bind textures on corresponding texture units
	/*glFrontFace(GL_CW);*/
	/*glCullFace(GL_BACK);*/
	/*glEnable(GL_CULL_FACE);*/
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAOs[objFirst]);
	glBindTexture(GL_TEXTURE_2D, textures[textFirst]);
	//glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

// Resize viewport on window resize
void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	 glViewport(0, 0, width, height);
}

// Process latest input received
void processReceivedInput(GLFWwindow* window)
{
	// Close window when escape is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//----------------------------------------------------------------------------
//
// main
//
int main()
{
	glfwInit();

	// Using OpenGL 4.5 Core 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window{glfwCreateWindow(screenWidth, screenHeight, "Textured Cube", nullptr, nullptr)};
	if (!window)
	{
		std::cout << "Failed to create GLFW window.\n";
		
		glfwTerminate();
		
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Viewport
	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window,frameBufferResizeCallback);
	
	glewInit();

	init();

	while (!glfwWindowShouldClose(window))
	{
		processReceivedInput(window);
		
		// uncomment to draw only wireframe
		// 
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		update();
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
