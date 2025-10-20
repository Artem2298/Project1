#pragma once
#include <GL/glew.h>
#include <string>

class Shader
{
private:
    GLuint shaderID;
    GLenum shaderType;
    std::string shaderPath;

    std::string readFile(const std::string& filePath);
    bool checkCompilation();

public:
    Shader();
    ~Shader();

    bool createShader(GLenum type, const std::string& sourceCode);

    bool createShaderFromFile(GLenum type, const std::string& filePath);

    void attachToProgram(GLuint programID);

    GLuint getID() const { return shaderID; }
    GLenum getType() const { return shaderType; }
    const std::string& getPath() const { return shaderPath; }

    void deleteShader();
};
