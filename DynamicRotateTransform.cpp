#include "DynamicRotateTransform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

DynamicRotateTransform::DynamicRotateTransform(const glm::vec3& rotAxis, float speed)
    : axis(glm::normalize(rotAxis)), currentAngle(0.0f), rotationSpeed(speed)
{
}

DynamicRotateTransform::~DynamicRotateTransform()
{
}

glm::mat4 DynamicRotateTransform::getMatrix() const
{
    return glm::rotate(glm::mat4(1.0f), glm::radians(currentAngle), axis);
}

void DynamicRotateTransform::update(float deltaTime)
{
    currentAngle += rotationSpeed * deltaTime;
    if (currentAngle >= 360.0f)
        currentAngle -= 360.0f;
}

void DynamicRotateTransform::setRotationSpeed(float speed)
{
    rotationSpeed = speed;
}