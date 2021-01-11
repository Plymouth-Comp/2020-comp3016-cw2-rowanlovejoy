#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <sstream>

class Shader
{
public:
    // Compile and link the given shaders to a create a usable shader program
    Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = std::string{});

	~Shader();

    // Active the shader for use -- shader must be active before setting uniform values
    void use() const;
	
    // Utility functions to set shader uniform values
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec2& value) const;
    void setUniform(const std::string& name, float x, float y) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, float x, float y, float z) const;
    void setUniform(const std::string& name, const glm::vec4& value) const;
    void setUniform(const std::string& name, float x, float y, float z, float w) const;
    void setUniform(const std::string& name, const glm::mat2& value) const;
    void setUniform(const std::string& name, const glm::mat3& value) const;
    void setUniform(const std::string& name, const glm::mat4& value) const;

	// Get the id of the shader program
	inline unsigned int getId() const
	{
	    return Id;
    }

private:
    unsigned int Id;
	
    // Check for shader compilation and linking errors 
    static void checkShaderErrors(const GLuint& shader, const std::string& type);
};
