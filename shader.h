#pragma once

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <string>

class Shader
{
public:
	// Constructor that builds the shader program
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	// Destructor that deletes the shader program
	~Shader();

	// Activate the shader program
	void use() const;
	
	// Set the value of the specified uniform
	void setUniform(const std::string& name, const bool value) const;
	void setUniform(const std::string& name, const int value) const;
	void setUniform(const std::string& name, const float value) const;
	void setUniform(const std::string& name, const glm::mat4& value) const;

	// Get the ID of the shader program
	inline unsigned int getProgram() const
	{
		return shaderProgram;
	}
private:
	// The ID of the shaderProgram
	unsigned int shaderProgram{};
};
