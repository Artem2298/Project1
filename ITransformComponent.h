#pragma once
#include <glm/mat4x4.hpp>

class ITransformComponent
{
public:
    virtual ~ITransformComponent() = default;
    virtual glm::mat4 getMatrix() const = 0;
    virtual void update(float deltaTime) {}
};