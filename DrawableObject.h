#pragma once
#include "Model.h"
#include "Transformation.h"
#include "ShaderProgram.h"
#include "ModelLoader.h"
#include <glm/vec3.hpp>

class DrawableObject
{
protected:
    Model model;
    Transformation transform;
    ModelLoader modelLoader;
    ShaderProgram* shader;

    glm::vec3 objectColor;
    float shininess;

public:
    DrawableObject(bool isDynamic = false);
    virtual ~DrawableObject();

    virtual void update(float deltaTime);
    virtual void draw();

    bool loadModel(const std::string& filePath, const std::string& arrayName);

    void addStaticTransform(ITransformComponent* component);
    void addDynamicTransform(ITransformComponent* component);

    void setShader(ShaderProgram* shaderProgram);
    ShaderProgram* getShader() const { return shader; }

    void setObjectColor(const glm::vec3& color) { objectColor = color; }
    glm::vec3 getObjectColor() const { return objectColor; }

    void setShininess(float shine) { shininess = shine; }
    float getShininess() const { return shininess; }

    Transformation& getTransformation() { return transform; }
    const Transformation& getTransformation() const { return transform; }

    Model& getModel() { return model; }
    const Model& getModel() const { return model; }

    glm::mat4 getModelMatrix() const { return transform.getMatrix(); }
};