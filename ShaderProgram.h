#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Shader.h"
#include "CameraObserver.h"

class Camera;

class ShaderProgram : public CameraObserver
{
private:
    GLuint programID;
    std::vector<std::unique_ptr<Shader>> shaders;

    GLint attribPosition;  
    GLint attribNormal;
    GLint attribTexCoord;

    bool checkLinking();

    void queryAttributeLocations();

public:
    ShaderProgram();
    ~ShaderProgram();

    bool loadFromFiles(const std::string& vertexPath,
        const std::string& fragmentPath);

    bool addShader(GLenum type, const std::string& filePath);
    bool addShaderFromSource(GLenum type, const std::string& sourceCode);

    bool link();

    void use() const;
    void unuse() const;

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

    void onCameraChanged(Camera* camera) override;

    GLuint getID() const { return programID; }

    GLint getPositionAttribLocation() const { return attribPosition; }
    GLint getNormalAttribLocation() const { return attribNormal; }
    GLint getTexCoordAttribLocation() const { return attribTexCoord; }
};

