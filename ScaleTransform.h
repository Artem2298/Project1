#pragma once
#include "ITransformComponent.h"
#include <glm/vec3.hpp>

class ScaleTransform : public ITransformComponent
{
private:
    glm::vec3 scaleValue;

public:
    ScaleTransform(const glm::vec3& scale);
    ~ScaleTransform();
    glm::mat4 getMatrix() const override;
    void setScale(const glm::vec3& scale);
    glm::vec3 getScale() const { return scaleValue; }
};