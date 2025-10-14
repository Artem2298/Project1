#pragma once
#include "ITransformComponent.h"
#include <glm/vec3.hpp>

class TranslateTransform : public ITransformComponent
{
private:
    glm::vec3 position;

public:
    TranslateTransform(const glm::vec3& pos);
    ~TranslateTransform();
    glm::mat4 getMatrix() const override;
    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition() const { return position; }
};