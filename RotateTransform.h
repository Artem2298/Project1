#pragma once
#include "ITransformComponent.h"
#include <glm/vec3.hpp>

class RotateTransform : public ITransformComponent
{
private:
    glm::vec3 axis;
    float angle;

public:
    RotateTransform(const glm::vec3& rotAxis, float rotAngle);
    ~RotateTransform();
    glm::mat4 getMatrix() const override;
    void setRotation(const glm::vec3& rotAxis, float rotAngle);
    void setAngle(float rotAngle);
};