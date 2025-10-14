#include "Square.h"

Square::Square()
{
    const float squareVertices[72] = {
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,

         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 1.0f,
         0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.5f, 0.5f,

         0.5f,  0.5f, 0.0f,  1.0f, 0.5f, 0.5f,
         0.5f, -0.5f, 0.0f,  0.5f, 1.0f, 0.5f,
        -0.5f, -0.5f, 0.0f,  0.5f, 0.5f, 1.0f
    };

    model.loadWithStride(squareVertices, 12, 6);
}

Square::~Square()
{
}

void Square::update(float deltaTime)
{
}