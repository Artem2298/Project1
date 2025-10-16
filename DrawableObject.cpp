#include "DrawableObject.h"
#include "ModelCache.h"

DrawableObject::DrawableObject(bool isDynamic)
    : transform(isDynamic)
{
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::update(float deltaTime)
{
    transform.update(deltaTime);
}

void DrawableObject::draw(ShaderProgram& shader)
{
    shader.use();
    shader.setUniform("modelMatrix", transform.getMatrix());
    model.draw();
}

bool DrawableObject::loadModel(const std::string& filePath, const std::string& arrayName)
{
    auto modelData = ModelCache::getInstance().loadModel(filePath, arrayName);

    if (!modelData)
    {
        std::cerr << "ERROR: Failed to load model from cache: " << filePath << "\n";
        return false;
    }

    model.loadWithStride(modelData->vertices.data(),
        modelData->vertexCount,
        modelData->stride);

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