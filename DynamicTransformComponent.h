#pragma once
#include "ITransformComponent.h"
#include <vector>
#include <memory>

class DynamicTransformComponent : public ITransformComponent
{
private:
    std::vector<std::unique_ptr<ITransformComponent>> components;

public:
    DynamicTransformComponent();
    ~DynamicTransformComponent();
    void add(ITransformComponent* component);
    glm::mat4 getMatrix() const override;
    void update(float deltaTime) override;
};