#include "ModelCache.h"
#include <iostream>

ModelCache* ModelCache::instance = nullptr;

ModelData::ModelData(const std::vector<float>& verts, unsigned int count, unsigned int str)
    : vertices(verts), vertexCount(count), stride(str)
{
}

ModelCache::ModelCache()
{
}

ModelCache::~ModelCache()
{
}

std::string ModelCache::generateKey(const std::string& filePath, const std::string& arrayName) const
{
    return filePath + ":" + arrayName;
}

ModelCache& ModelCache::getInstance()
{
    if (!instance)
        instance = new ModelCache();
    return *instance;
}

std::shared_ptr<ModelData> ModelCache::loadModel(const std::string& filePath, const std::string& arrayName)
{
    std::string key = generateKey(filePath, arrayName);

    auto it = cache.find(key);
    if (it != cache.end())
    {
        return it->second;
    }

    std::vector<float> vertices = loader.loadFromHeader(filePath, arrayName);

    if (vertices.empty())
    {
        return nullptr;
    }

    unsigned int stride = 6;
    unsigned int vertexCount = vertices.size() / stride;

    if (vertices.size() % stride != 0)
    {
        std::cerr << "WARNING: Vertex data size not perfectly divisible by " << stride << "\n";
    }

    auto modelData = std::make_shared<ModelData>(vertices, vertexCount, stride);
    cache[key] = modelData;

    return modelData;
}

std::shared_ptr<ModelData> ModelCache::loadModelFromText(const std::string& filePath)
{
    std::string key = "text:" + filePath;

    auto it = cache.find(key);
    if (it != cache.end())
    {
        std::cout << "\nModel cache HIT: " << key << "\n";
        return it->second;
    }

    std::cout << "\nModel cache MISS: " << key << " - loading from text...\n";

    std::vector<float> vertices = loader.loadFromText(filePath);

    if (vertices.empty())
    {
        std::cerr << "ERROR: Failed to load model from text file: " << filePath << "\n";
        return nullptr;
    }

    auto modelData = std::make_shared<ModelData>(vertices, vertices.size(), 8);
    cache[key] = modelData;

    std::cout << "Model cached: " << key << " (" << modelData->vertexCount << " vertices)" << std::endl;

    return modelData;
}

std::shared_ptr<ModelData> ModelCache::loadModelFromOBJ(const std::string& filePath)
{
    std::string key = "obj:" + filePath;

    auto it = cache.find(key);
    if (it != cache.end())
    {
        std::cout << "\nModel cache HIT: " << key << "\n";
        return it->second;
    }

    std::cout << "\nModel cache MISS: " << key << " - loading from OBJ...\n";

    std::vector<float> vertices = loader.loadFromOBJ(filePath);

    if (vertices.empty())
    {
        std::cerr << "ERROR: Failed to load model from OBJ file: " << filePath << "\n";
        return nullptr;
    }

    unsigned int stride = 8;
    unsigned int vertexCount = vertices.size() / stride;

    if (vertices.size() % stride != 0)
    {
        std::cerr << "WARNING: Vertex data size not perfectly divisible by " << stride
            << " (size: " << vertices.size() << ")\n";
    }

    auto modelData = std::make_shared<ModelData>(vertices, vertexCount, stride);
    cache[key] = modelData;

    std::cout << "Model cached: " << key << " (" << vertexCount << " vertices)\n";
    return modelData;
}

void ModelCache::clear()
{
    cache.clear();
    std::cout << "Model cache cleared\n";
}

void ModelCache::printStats() const
{
    size_t totalVertices = 0;
    size_t totalBytes = 0;

    for (const auto& pair : cache)
    {
        totalVertices += pair.second->vertexCount;
        totalBytes += pair.second->vertices.size() * sizeof(float);
    }

    std::cout << "Total vertices: " << totalVertices << "\n";
    std::cout << "Total memory: " << totalBytes / 1024 << " KB\n";
    std::cout << "========================\n";
}

void ModelCache::destroy()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}