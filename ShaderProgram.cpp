#include "ShaderProgram.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShaderProgram::ShaderProgram()
    : programID(0)
{
    programID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

bool ShaderProgram::loadFromFiles(const std::string& vertexPath,
    const std::string& fragmentPath)
{
    if (!addShader(GL_VERTEX_SHADER, vertexPath))
    {
        return false;
    }

    if (!addShader(GL_FRAGMENT_SHADER, fragmentPath))
    {
        return false;
    }

    if (!link())
    {
        return false;
    }

    return true;
}

bool ShaderProgram::addShader(GLenum type, const std::string& filePath)
{
    auto shader = std::make_unique<Shader>();

    if (!shader->createShaderFromFile(type, filePath))
    {
        return false;
    }

    shader->attachToProgram(programID);
    shaders.push_back(std::move(shader));

    return true;
}

bool ShaderProgram::addShaderFromSource(GLenum type, const std::string& sourceCode)
{
    auto shader = std::make_unique<Shader>();

    if (!shader->createShader(type, sourceCode))
    {
        return false;
    }

    shader->attachToProgram(programID);
    shaders.push_back(std::move(shader));

    return true;
}

bool ShaderProgram::link()
{
    glLinkProgram(programID);
    return checkLinking();
}

bool ShaderProgram::checkLinking()
{
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(programID, infoLogLength, NULL, infoLog);

        std::cerr << "ERROR: Shader program linking failed!\n";
        std::cerr << "Log: " << infoLog << "\n";

        delete[] infoLog;
        return false;
    }

    std::cout << "Shader program linked successfully (ID: " << programID << ")\n";
    return true;
}

void ShaderProgram::use() const
{
    glUseProgram(programID);
}

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
    GLint location = glGetUniformLocation(programID, name.c_str());

    return location;
}

void ShaderProgram::onCameraChanged(Camera* camera)
{
    if (!camera) return;

    use();
    setUniform("viewMatrix", camera->getCamera());
    setUniform("projectionMatrix", camera->getProjectionMatrix());

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