#pragma once
#include "Model.h"
#include "Transformation.h"
#include "ShaderProgram.h"
#include <glm/vec3.hpp>

class DrawableObject
{
protected:
    Model model;
    Transformation transform;

public:
    DrawableObject(bool isDynamic = false);
    virtual ~DrawableObject();

    virtual void update(float deltaTime);
    virtual void draw(ShaderProgram& shader);

    void addStaticTransform(ITransformComponent* component);
    void addDynamicTransform(ITransformComponent* component);

    Transformation& getTransformation() { return transform; }
    const Transformation& getTransformation() const { return transform; }

    Model& getModel() { return model; }
    const Model& getModel() const { return model; }

    glm::mat4 getModelMatrix() const { return transform.getMatrix(); }
};