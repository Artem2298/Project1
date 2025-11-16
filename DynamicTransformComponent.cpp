#include "DynamicTransformComponent.h"

DynamicTransformComponent::DynamicTransformComponent()
{
}

DynamicTransformComponent::~DynamicTransformComponent()
{
}

void DynamicTransformComponent::add(ITransformComponent* component)
{
    if (component)
    {
        components.push_back(std::unique_ptr<ITransformComponent>(component));
    }
}

glm::mat4 DynamicTransformComponent::getMatrix() const
{
    glm::mat4 result = glm::mat4(1.0f);
    for (const auto& component : components)
    {
        result = result * component->getMatrix();
    }
    return result;
}

void DynamicTransformComponent::update(float deltaTime)
{
    for (auto& component : components)
    {
        component->update(deltaTime);
    }
}