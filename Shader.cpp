#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
    : shaderID(0), shaderType(0), shaderPath("")
{
}

Shader::~Shader()
{
    deleteShader();
}

std::string Shader::readFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "ERROR: Unable to open shader file: " << filePath << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

bool Shader::checkCompilation()
{
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);

        std::string typeStr = (shaderType == GL_VERTEX_SHADER) ? "VERTEX" :
            (shaderType == GL_FRAGMENT_SHADER) ? "FRAGMENT" :
            (shaderType == GL_GEOMETRY_SHADER) ? "GEOMETRY" : "UNKNOWN";

        std::cerr << "ERROR: " << typeStr << " Shader compilation failed!\n";
        std::cerr << "File: " << shaderPath << "\n";
        std::cerr << "Log: " << infoLog << "\n";

        delete[] infoLog;
        return false;
    }

    std::string typeStr = (shaderType == GL_VERTEX_SHADER) ? "Vertex" :
        (shaderType == GL_FRAGMENT_SHADER) ? "Fragment" :
        (shaderType == GL_GEOMETRY_SHADER) ? "Geometry" : "Unknown";

    //std::cout << typeStr << " shader compiled successfully";
    /*if (!shaderPath.empty()) {
        std::cout << " [" << shaderPath << "]";
    }
    std::cout << "\n";*/

    return true;
}

bool Shader::createShader(GLenum type, const std::string& sourceCode)
{
    if (sourceCode.empty())
    {
        std::cerr << "ERROR: Shader source code is empty!\n";
        return false;
    }

    shaderType = type;
    shaderID = glCreateShader(type);

    const char* src = sourceCode.c_str();
    glShaderSource(shaderID, 1, &src, NULL);
    glCompileShader(shaderID);

    return checkCompilation();
}

bool Shader::createShaderFromFile(GLenum type, const std::string& filePath)
{
    shaderPath = filePath;
    std::string sourceCode = readFile(filePath);

    if (sourceCode.empty())
    {
        return false;
    }

    return createShader(type, sourceCode);
}

void Shader::attachToProgram(GLuint programID)
{
    if (shaderID != 0 && programID != 0)
    {
        glAttachShader(programID, shaderID);
    }
    else
    {
        std::cerr << "ERROR: Cannot attach shader (shaderID="
            << shaderID << ", programID=" << programID << ")\n";
    }
}

void Shader::deleteShader()
{
    if (shaderID != 0)
    {
        glDeleteShader(shaderID);
        shaderID = 0;
    }
}