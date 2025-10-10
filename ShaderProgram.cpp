#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::use() const
{
    glUseProgram(shaderProgramID);
}

void ShaderProgram::setUniform1f(const std::string& name, float value)
{
    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
}

void ShaderProgram::setUniform1i(const std::string& name, int value)
{
    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}

void ShaderProgram::setUniform3f(const std::string& name, float x, float y, float z)
{
    GLint location = getUniformLocation(name);
    glUniform3f(location, x, y, z);
}

void ShaderProgram::setUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
{
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
    GLint location = glGetUniformLocation(shaderProgramID, name.c_str());

    if (location == -1)
    {
        std::cerr << "WARNING: Uniform '" << name << "' not found in shader program" << std::endl;
    }

    return location;
}