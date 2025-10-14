#pragma once
#include "ShaderLoader.h"
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class ShaderProgram : public ShaderLoader
{
public:
    ShaderProgram();
    ~ShaderProgram();
    void use() const;

    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, int value);

    void setUniform(const std::string& name, float x, float y);
    void setUniform(const std::string& name, float x, float y, float z);
    void setUniform(const std::string& name, float x, float y, float z, float w);

    void setUniform(const std::string& name, const glm::vec2& vec);
    void setUniform(const std::string& name, const glm::vec3& vec);
    void setUniform(const std::string& name, const glm::vec4& vec);

    void setUniform(const std::string& name, const glm::mat3& matrix);
    void setUniform(const std::string& name, const glm::mat4& matrix);

    void setUniform(const std::string& name, bool value);

private:
    GLint getUniformLocation(const std::string& name);
};