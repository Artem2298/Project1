#pragma once
#include "ShaderLoader.h"
#include <glm/mat4x4.hpp>

class ShaderProgram : public ShaderLoader
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void use() const;

    void setUniform1f(const std::string& name, float value);
    void setUniform1i(const std::string& name, int value);
    void setUniform3f(const std::string& name, float x, float y, float z);
    void setUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

private:
    GLint getUniformLocation(const std::string& name);
};