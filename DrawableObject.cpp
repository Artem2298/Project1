#include "DrawableObject.h"
#include "ModelCache.h"

DrawableObject::DrawableObject(bool isDynamic)
    : transform(isDynamic),
    shader(nullptr),
    objectColor(1.0f, 1.0f, 1.0f),
    shininess(32.0f)
{
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::update(float deltaTime)
{
    transform.update(deltaTime);
}

void DrawableObject::draw()
{
    if (!model.isModelLoaded()) {
        std::cerr << "DrawableObject::draw() - Model not loaded!" << std::endl;
        return;
    }

    if (shader == nullptr) {
        std::cerr << "DrawableObject::draw() - No shader assigned!" << std::endl;
        return;
    }

    model.draw();
}

bool DrawableObject::loadModel(const std::string& filePath, const std::string& arrayName)
{
    std::shared_ptr<ModelData> modelData = ModelCache::getInstance().loadModel(filePath, arrayName);

    if (!modelData || modelData->vertices.empty()) {
        std::cerr << "Failed to load model from cache: " << filePath << " (" << arrayName << ")" << std::endl;
        return false;
    }

    model.loadWithStride(modelData->vertices.data(), modelData->vertices.size(), modelData->stride, shader);

    return true;
}

bool DrawableObject::loadModelFromText(const std::string& filePath)
{
    std::shared_ptr<ModelData> modelData = ModelCache::getInstance().loadModelFromText(filePath);

    if (!modelData || modelData->vertices.empty()) {
        std::cerr << "Failed to load model from text: " << filePath << std::endl;
        return false;
    }

    model.loadWithStride(modelData->vertices.data(), modelData->vertices.size(), modelData->stride, shader);
    return true;
}

bool DrawableObject::loadModelFromOBJ(const std::string& filePath)
{
    std::shared_ptr<ModelData> modelData = ModelCache::getInstance().loadModelFromOBJ(filePath);

    if (!modelData || modelData->vertices.empty()) {
        std::cerr << "Failed to load model from OBJ: " << filePath << std::endl;
        return false;
    }

    model.loadWithStride(modelData->vertices.data(), modelData->vertices.size(), modelData->stride, shader);
    return true;
}

void DrawableObject::addStaticTransform(ITransformComponent* component)
{
    transform.addStatic(component);
}

void DrawableObject::addDynamicTransform(ITransformComponent* component)
{
    transform.addDynamic(component);
}

void DrawableObject::setShader(ShaderProgram* shaderProgram)
{
    shader = shaderProgram;
}