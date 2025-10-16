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
        //std::cout << "Model cache HIT: " << key << "\n";
        return it->second;
    }

    //std::cout << "Model cache MISS: " << key << " - loading...\n";
    std::vector<float> vertices = loader.loadFromHeader(filePath, arrayName);

    if (vertices.empty())
    {
        //std::cerr << "ERROR: Failed to load model from " << filePath << "\n";
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

    //std::cout << "Model cached: " << key << " (" << vertexCount << " vertices)\n";
    return modelData;
}

void ModelCache::clear()
{
    cache.clear();
    std::cout << "Model cache cleared\n";
}

void ModelCache::printStats() const
{
    std::cout << "=== Model Cache Stats ===\n";
    std::cout << "Cached models: " << cache.size() << "\n";

    size_t totalVertices = 0;
    size_t totalBytes = 0;

    for (const auto& pair : cache)
    {
        totalVertices += pair.second->vertexCount;
        totalBytes += pair.second->vertices.size() * sizeof(float);
        std::cout << "  - " << pair.first << ": "
            << pair.second->vertexCount << " vertices\n";
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