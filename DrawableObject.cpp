#include "DrawableObject.h"

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
    shader.setUniform("model", transform.getMatrix());
    model.draw();
}

bool DrawableObject::loadModel(const std::string& filePath, const std::string& arrayName)
{
    std::vector<float> vertices = modelLoader.loadFromHeader(filePath, arrayName);

    if (vertices.empty())
    {
        std::cerr << "ERROR: Failed to load model from " << filePath << "\n";
        return false;
    }

    unsigned int vertexCount = vertices.size() / 6;

    if (vertices.size() % 6 != 0)
    {
        std::cerr << "ERROR: Vertex data size not divisible by 6\n";
        return false;
    }

    model.loadWithStride(vertices.data(), vertexCount, 6);
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