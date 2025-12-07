#pragma once
#include "ITransformComponent.h"
#include <glm/mat4x4.hpp>

class CustomMatrixTransform : public ITransformComponent
{
private:
    float wValue;

public:
    CustomMatrixTransform(float w = 20.0f);
    ~CustomMatrixTransform() = default;

    glm::mat4 getMatrix() const override;
};