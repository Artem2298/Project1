#include "Triangle.h"

Triangle::Triangle() : rotationSpeed(50.0f)
{
    float triangleVertices[] = {
        0.0f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    model.load(triangleVertices, 3);

    color = glm::vec3(0.5f, 0.0f, 0.5f);
}

Triangle::~Triangle()
{
}

void Triangle::update(float deltaTime)
{
    transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), rotationSpeed * deltaTime);
}