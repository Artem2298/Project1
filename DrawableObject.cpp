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

    shader->use();

    glm::mat4 modelMatrix = transform.getMatrix();
    shader->setUniform("modelMatrix", modelMatrix);

    GLint colorLoc = shader->getUniformLocation("objectColor");
    if (colorLoc != -1) {
        shader->setUniform("objectColor", objectColor);
    }

    GLint shininessLoc = shader->getUniformLocation("shininess");
    if (shininessLoc != -1) {
        shader->setUniform("shininess", shininess);
    }

    model.draw();
}

bool DrawableObject::loadModel(const std::string& filePath, const std::string& arrayName)
{
    // Use ModelCache to avoid reloading the same file multiple times
    std::shared_ptr<ModelData> modelData = ModelCache::getInstance().loadModel(filePath, arrayName);

    if (!modelData || modelData->vertices.empty()) {
        std::cerr << "Failed to load model from cache: " << filePath << " (" << arrayName << ")" << std::endl;
        return false;
    }

    //std::cout << "Loaded from cache: " << filePath << " - "
    //    << modelData->vertices.size() << " floats" << std::endl;

    // Load into GPU
    model.loadWithStride(modelData->vertices.data(), modelData->vertices.size(), modelData->stride);

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