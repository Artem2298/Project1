#include "StaticTransformComponent.h"

StaticTransformComponent::StaticTransformComponent()
    : cachedMatrix(1.0f), isDirty(true)
{
}

StaticTransformComponent::~StaticTransformComponent()
{
}

void StaticTransformComponent::add(ITransformComponent* component)
{
    if (component)
    {
        components.push_back(std::unique_ptr<ITransformComponent>(component));
        isDirty = true;
    }
}

void StaticTransformComponent::updateCachedMatrix()
{
    cachedMatrix = glm::mat4(1.0f);
    for (const auto& component : components)
    {
        cachedMatrix = cachedMatrix * component->getMatrix();
    }
    isDirty = false;
}

glm::mat4 StaticTransformComponent::getMatrix() const
{
    if (isDirty)
    {
        const_cast<StaticTransformComponent*>(this)->updateCachedMatrix();
    }
    return cachedMatrix;
}

void StaticTransformComponent::update(float deltaTime)
{
    for (auto& component : components)
    {
        component->update(deltaTime);
    }
    isDirty = true;
}