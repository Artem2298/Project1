#include "Triangle.h"
#include "DynamicRotateTransform.h"
#include "TranslateTransform.h"

Triangle::Triangle() : DrawableObject(true), rotateTransform(nullptr)
{
    const float triangleVertices[54] = {
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,

        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f
    };

    model.loadWithStride(triangleVertices, 6, 6);

    rotateTransform = new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 50.0f);
    addDynamicTransform(rotateTransform);
    addStaticTransform(new TranslateTransform(glm::vec3(-0.5f, 0.5f, 0.0f)));
}

Triangle::~Triangle()
{
}

void Triangle::update(float deltaTime)
{
    DrawableObject::update(deltaTime);
}

void Triangle::setRotationSpeed(float speed)
{
    if (rotateTransform)
        rotateTransform->setRotationSpeed(speed);
}