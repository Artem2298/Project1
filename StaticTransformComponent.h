#pragma once
#include "ITransformComponent.h"
#include <vector>
#include <memory>

class StaticTransformComponent : public ITransformComponent
{
private:
    std::vector<std::unique_ptr<ITransformComponent>> components;
    glm::mat4 cachedMatrix;
    bool isDirty;

    void updateCachedMatrix();

public:
    StaticTransformComponent();
    ~StaticTransformComponent();
    void add(ITransformComponent* component);
    glm::mat4 getMatrix() const override;
    void update(float deltaTime) override;
};