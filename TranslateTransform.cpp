#include "TranslateTransform.h"
#include <glm/gtc/matrix_transform.hpp>

TranslateTransform::TranslateTransform(const glm::vec3& pos)
    : position(pos)
{
}

TranslateTransform::~TranslateTransform()
{
}

glm::mat4 TranslateTransform::getMatrix() const
{
    return glm::translate(glm::mat4(1.0f), position);
}

void TranslateTransform::setPosition(const glm::vec3& pos)
{
    position = pos;
}