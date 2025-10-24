#include "DynamicTranslateTransform.h"
#include <glm/gtc/matrix_transform.hpp>

DynamicTranslateTransform::DynamicTranslateTransform(const glm::vec3& pos, const glm::vec3& vel)
    : position(pos)
    , velocity(vel)
{
}

DynamicTranslateTransform::~DynamicTranslateTransform()
{
}

glm::mat4 DynamicTranslateTransform::getMatrix() const
{
    return glm::translate(glm::mat4(1.0f), position);
}

void DynamicTranslateTransform::update(float deltaTime)
{
    if (glm::length(velocity) > 0.0001f) {
        position += velocity * deltaTime;
    }
}

void DynamicTranslateTransform::setVelocity(const glm::vec3& vel)
{
    velocity = vel;
}

void DynamicTranslateTransform::setPosition(const glm::vec3& pos)
{
    position = pos;
}