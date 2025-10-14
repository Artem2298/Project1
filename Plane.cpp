#include "Plane.h"
#include "TranslateTransform.h"

Plane::Plane() : DrawableObject(false)
{
    const float planeVertices[36] = {
        1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
       -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,

       -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
       -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f
    };

    model.loadWithStride(planeVertices, 6, 6);
    addStaticTransform(new TranslateTransform(glm::vec3(0.0f, -1.0f, 0.0f)));
}

Plane::~Plane()
{
}

void Plane::update(float deltaTime)
{
    DrawableObject::update(deltaTime);
}