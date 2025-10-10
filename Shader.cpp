#include "Shader.h"
#include <iostream>

Shader::Shader(GLuint shaderType, const std::string& sourceCode)
    : type(shaderType)
{
    id = glCreateShader(type);
    const char* src = sourceCode.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
}

Shader::~Shader()
{
    glDeleteShader(id);
}
