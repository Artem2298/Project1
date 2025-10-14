#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>

class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();

    std::vector<float> loadFromHeader(const std::string& filePath, const std::string& arrayName);

private:
    std::string readFile(const std::string& filePath);
    std::vector<float> parseFloatArray(const std::string& content, const std::string& arrayName);
};