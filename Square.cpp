#include "Square.h"

Square::Square()
{
    float squareVertices[] = {
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,

         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    model.load(squareVertices, 6);

    color = glm::vec3(0.0f, 0.5f, 1.0f);
}

Square::~Square()
{
}

void Square::update(float deltaTime)
{
}