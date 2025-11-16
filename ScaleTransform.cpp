#include "ScaleTransform.h"
#include <glm/gtc/matrix_transform.hpp>

ScaleTransform::ScaleTransform(const glm::vec3& scale)
    : scaleValue(scale)
{
}

ScaleTransform::~ScaleTransform()
{
}

glm::mat4 ScaleTransform::getMatrix() const
{
    return glm::scale(glm::mat4(1.0f), scaleValue);
}

void ScaleTransform::setScale(const glm::vec3& scale)
{
    scaleValue = scale;
}