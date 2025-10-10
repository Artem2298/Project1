#pragma once
#include <string>
#include <GL/glew.h>

class Shader
{
private:
    GLuint id;
    GLenum type;

public:
    Shader(GLuint shaderType, const std::string& sourceCode);
    ~Shader();
    GLuint getID() const { return id; }
    GLenum getType() const { return type; }
};