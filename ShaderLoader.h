#pragma once
#include <string>
#include <GL/glew.h>
#include "Shader.h"

class ShaderLoader
{
protected:
    GLuint shaderProgramID;

    std::string readFile(const std::string& filePath);

    bool checkShaderCompilation(GLuint shader, const std::string& shaderName);

    bool checkProgramLinking();

public:
    ShaderLoader();
    virtual ~ShaderLoader();

    bool loadShader(const std::string& vertexPath, const std::string& fragmentPath);

    GLuint getProgramID() const { return shaderProgramID; }
};