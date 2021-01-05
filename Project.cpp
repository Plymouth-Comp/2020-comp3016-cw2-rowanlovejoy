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
#include <glm/glm.hpp> //includes GLM
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#include <iostream>

#define BUFFER_OFFSET(a) ((void*)(a))

enum VAO_IDs{Triangles, Indices, Colours, Tex, NumVAOs = 1};
enum Buffer_IDs{ArrayBuffer, NumBuffers = 4};
enum Attrib_IDs{vPosition = 0, cPosition = 1, tPosition = 2};

GLuint VAOs[NumVAOs]{};
GLuint Buffers[NumBuffers]{};
GLuint texture1{};

constexpr GLuint NumVertices{36};

constexpr GLuint screenWidth{800};
constexpr GLuint screenHeight{600};

//----------------------------------------------------------------------------
//
// init
//
void init()
{
	// GLEW provides functionality to load shaders from files
	ShaderInfo shaders[]
	{
		{GL_VERTEX_SHADER, "media/triangles.vert"},
		{GL_FRAGMENT_SHADER, "media/triangles.frag"},
		{GL_NONE, nullptr}
	};

	const auto shaderProgram{LoadShaders(shaders)};
	glUseProgram(shaderProgram);

	constexpr GLfloat vertices[][3]
	{
		{0.5f,  0.5f, -0.5f},  //0 top right
		{0.5f, -0.5f, -0.5f},  //1 bottom right
		{-0.5f, -0.5f, -0.5f}, //2 bottom left
		{-0.5f,  0.5f, -0.5f},  //3 top left

		{0.5f,  0.5f, 0.5f},  //4 top right
		{0.5f, -0.5f, 0.5f},  //5 bottom right
		{-0.5f, -0.5f, 0.5f}, //6 bottom left
		{-0.5f,  0.5f, 0.5f}  //7 top left 
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

	constexpr GLfloat colours[][4]
	{
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},  
		{1.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f}, 
		{0.0f, 0.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 0.0f, 1.0f} 
	};
	
	constexpr GLfloat textureCoords[][2]
	{
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f}
	};

	// Buffer initialisation
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);
	
	glGenBuffers(NumBuffers, Buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT,GL_FALSE, 0, BUFFER_OFFSET(0));
	
	//Colour Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);

	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	loadTexture(texture1, "media/textures/awesomeface.png");
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

	// creating the model matrix
	auto model{glm::mat4{1.0f}};
	model = glm::scale(model, glm::vec3{2.0f, 2.0f, 2.0f});
	model = glm::rotate(model, glm::radians(-40.0f), glm::vec3{1.0f, 0.0f, 0.0f});
	model = glm::translate(model, glm::vec3{0.0f, 0.0f, -1.0f});

	// creating the view matrix
	auto view{glm::mat4{1.0f}};
	view = glm::translate(view, glm::vec3{0.0f, 0.0f, -2.0f});

	// creating the projection matrix
	const auto projection{glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f)};

	// Adding all matrices up to create combined matrix
	const auto mvp{projection * view * model};

	//adding the Uniform to the shader
	const auto mvpLoc{glGetUniformLocation(shaderProgram, "mvp")};
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition); 
	glEnableVertexAttribArray(tPosition);
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

//----------------------------------------------------------------------------
//
// display
//
void display()
{
	static constexpr GLfloat black[]{0.0f, 0.0f, 0.0f, 0.0f};

	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);
	// bind textures on corresponding texture units
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(VAOs[Triangles]);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
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
		
	glewInit();

	init();

	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe 
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
