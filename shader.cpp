#include "shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Construct that builds the shader
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	// Retrieve vertex and fragment source code from the filePath
	std::string vertexCode{};
	std::string fragmentCode{};
	std::ifstream vShaderFile{};
	std::ifstream fShaderFile{};

	// Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		std::stringstream vShaderStream{};
		std::stringstream fShaderStream{};

		// Read the files' buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close the file streams
		vShaderFile.close();
		fShaderFile.close();

		// Convert streams into strings
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(const std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.code().value() << ": " << e.what() << "\n";
	}

	const auto vShaderCode{vertexCode.c_str()};
	const auto fShaderCode{fragmentCode.c_str()};

	// Compile shaders
	int success{};
	char infoLog[512]{};

	// Vertex shader
	const auto vertexShader{glCreateShader(GL_VERTEX_SHADER)};
	glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
	glCompileShader(vertexShader);
	// Print compile errors if any
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Fragment shader
	const auto fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
	glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentShader);
	// Print compilation errors if any
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Print linking errors if any
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}

	// Delete the individual shaders; after the shader program's been lineked, they're no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	// Delete shaderProgram when object is destroyed (is the correct?)
	glDeleteProgram(shaderProgram);
}

// Activate the shader program
void Shader::use() const
{
	glUseProgram(shaderProgram);
}

// Set the value of the specified uniform
void Shader::setUniform(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), static_cast<int>(value));
}

// Set the value of the specified uniform
void Shader::setUniform(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

// Set the value of the specified uniform
void Shader::setUniform(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

// Set the value of the specified uniform
void Shader::setUniform(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
