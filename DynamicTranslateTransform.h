#pragma once
#include "ITransformComponent.h"
#include <glm/vec3.hpp>

class DynamicTranslateTransform : public ITransformComponent
{
private:
    glm::vec3 position;
    glm::vec3 velocity;

public:
    DynamicTranslateTransform(const glm::vec3& pos, const glm::vec3& vel);
    ~DynamicTranslateTransform();
    glm::mat4 getMatrix() const override;
    void update(float deltaTime) override;
    void setVelocity(const glm::vec3& vel);
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getVelocity() const { return velocity; }
};