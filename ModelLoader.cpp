#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

std::string ModelLoader::readFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}

std::vector<float> ModelLoader::parseFloatArray(const std::string& content, const std::string& arrayName)
{
    std::vector<float> result;

    size_t arrayPos = content.find(arrayName);
    if (arrayPos == std::string::npos)
    {
        std::cerr << "ERROR: Array '" << arrayName << "' not found in file\n";
        return result;
    }

    size_t openBrace = content.find('{', arrayPos);
    if (openBrace == std::string::npos)
    {
        std::cerr << "ERROR: Could not find opening brace\n";
        return result;
    }

    size_t closeBrace = content.find('}', openBrace);
    if (closeBrace == std::string::npos)
    {
        std::cerr << "ERROR: Could not find closing brace\n";
        return result;
    }

    std::string arrayContent = content.substr(openBrace + 1, closeBrace - openBrace - 1);

    std::stringstream ss(arrayContent);
    std::string token;

    while (ss >> token)
    {
        if (token.empty() || token == ",")
            continue;

        try
        {
            float value = std::stof(token);
            result.push_back(value);
        }
        catch (const std::exception& e)
        {
            std::cerr << "WARNING: Could not parse '" << token << "' as float\n";
        }
    }

    std::cout << "Loaded array '" << arrayName << "': " << result.size() << " floats\n";
    return result;
}

std::vector<float> ModelLoader::loadFromHeader(const std::string& filePath, const std::string& arrayName)
{
    std::string content = readFile(filePath);

    if (content.empty())
    {
        return std::vector<float>();
    }

    return parseFloatArray(content, arrayName);
}