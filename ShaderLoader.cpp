#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

ShaderLoader::ShaderLoader() : shaderProgramID(0)
{
}

ShaderLoader::~ShaderLoader()
{
    if (shaderProgramID != 0)
        glDeleteProgram(shaderProgramID);
}

std::string ShaderLoader::readFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "ERROR: Could not open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

bool ShaderLoader::checkShaderCompilation(GLuint shader, const std::string& shaderName)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        std::cerr << "Shader compilation ERROR [" << shaderName << "]:\n" << strInfoLog << std::endl;

        delete[] strInfoLog;
        return false;
    }

    std::cout << "Shader [" << shaderName << "] compiled successfully." << std::endl;
    return true;
}

bool ShaderLoader::checkProgramLinking()
{
    GLint status;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shaderProgramID, infoLogLength, NULL, strInfoLog);

        std::cerr << "Program linking ERROR:\n" << strInfoLog << std::endl;

        delete[] strInfoLog;
        return false;
    }

    std::cout << "Shader program linked successfully." << std::endl;
    return true;
}

bool ShaderLoader::loadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    // Читаем содержимое файлов
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);

    if (vertexSource.empty() || fragmentSource.empty())
        return false;

    // Создаём и компилируем шейдеры
    Shader vertexShader(GL_VERTEX_SHADER, vertexSource);
    if (!checkShaderCompilation(vertexShader.getID(), "Vertex Shader"))
        return false;

    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!checkShaderCompilation(fragmentShader.getID(), "Fragment Shader"))
        return false;

    // Создаём программу
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader.getID());
    glAttachShader(shaderProgramID, fragmentShader.getID());
    glLinkProgram(shaderProgramID);

    // Проверяем линковку
    if (!checkProgramLinking())
        return false;

    return true;
}
