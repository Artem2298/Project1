#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "ModelLoader.h"

struct ModelData
{
    std::vector<float> vertices;
    unsigned int vertexCount;
    unsigned int stride;

    ModelData(const std::vector<float>& verts, unsigned int count, unsigned int str);
};

class ModelCache
{
private:
    static ModelCache* instance;
    std::map<std::string, std::shared_ptr<ModelData>> cache;
    ModelLoader loader;

    ModelCache();
    std::string generateKey(const std::string& filePath, const std::string& arrayName) const;

public:
    ~ModelCache();

    static ModelCache& getInstance();
    std::shared_ptr<ModelData> loadModel(const std::string& filePath, const std::string& arrayName);
    void clear();
    void printStats() const;
    static void destroy();
};