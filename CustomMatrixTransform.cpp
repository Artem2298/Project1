#include "CustomMatrixTransform.h"

CustomMatrixTransform::CustomMatrixTransform(float w)
    : wValue(w)
{
}

glm::mat4 CustomMatrixTransform::getMatrix() const
{
    glm::mat4 matrix(1.0f);

    matrix[3][3] = wValue;

    return matrix;
}