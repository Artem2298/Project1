#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "ShaderLoader.h"
#include "CameraObserver.h"

class Camera;

class ShaderProgram : public ShaderLoader, public CameraObserver
{
public:
    ShaderProgram();
    ~ShaderProgram();

    void onCameraChanged(Camera* camera) override;

    void use() const;
    GLint getUniformLocation(const std::string& name);

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
};