#include "RotateTransform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

RotateTransform::RotateTransform(const glm::vec3& rotAxis, float rotAngle)
    : axis(glm::normalize(rotAxis)), angle(rotAngle)
{
}

RotateTransform::~RotateTransform()
{
}

glm::mat4 RotateTransform::getMatrix() const
{
    return glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
}

void RotateTransform::setRotation(const glm::vec3& rotAxis, float rotAngle)
{
    axis = glm::normalize(rotAxis);
    angle = rotAngle;
}

void RotateTransform::setAngle(float rotAngle)
{
    angle = rotAngle;
}