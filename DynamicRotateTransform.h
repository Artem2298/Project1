#pragma once
#include "ITransformComponent.h"
#include <glm/vec3.hpp>

class DynamicRotateTransform : public ITransformComponent
{
private:
    glm::vec3 axis;
    float currentAngle;
    float rotationSpeed;

public:
    DynamicRotateTransform(const glm::vec3& rotAxis, float speed);
    ~DynamicRotateTransform();
    glm::mat4 getMatrix() const override;
    void update(float deltaTime) override;
    void setRotationSpeed(float speed);
    float getCurrentAngle() const { return currentAngle; }
};