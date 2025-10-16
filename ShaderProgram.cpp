#include "ShaderProgram.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::onCameraChanged(Camera* camera)
{
    if (!camera) return;

    use();
    setUniform("viewMatrix", camera->getCamera());
    setUniform("projectionMatrix", camera->getProjectionMatrix());
}

void ShaderProgram::use() const
{
    glUseProgram(shaderProgramID);
}

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
    GLint location = glGetUniformLocation(shaderProgramID, name.c_str());
    if (location == -1)
    {
        std::cerr << "WARNING: Uniform '" << name << "' not found\n";
    }
    return location;
}

void ShaderProgram::setUniform(const std::string& name, float value)
{
    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const std::string& name, int value)
{
    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const std::string& name, float x, float y)
{
    GLint location = getUniformLocation(name);
    glUniform2f(location, x, y);
}

void ShaderProgram::setUniform(const std::string& name, float x, float y, float z)
{
    GLint location = getUniformLocation(name);
    glUniform3f(location, x, y, z);
}

void ShaderProgram::setUniform(const std::string& name, float x, float y, float z, float w)
{
    GLint location = getUniformLocation(name);
    glUniform4f(location, x, y, z, w);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& vec)
{
    GLint location = getUniformLocation(name);
    glUniform2fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vec)
{
    GLint location = getUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& vec)
{
    GLint location = getUniformLocation(name);
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat3& matrix)
{
    GLint location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix)
{
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUniform(const std::string& name, bool value)
{
    GLint location = getUniformLocation(name);
    glUniform1i(location, (int)value);
}