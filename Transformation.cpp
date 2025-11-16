#include "Transformation.h"

Transformation::Transformation(bool isDynamic)
    : useDynamic(isDynamic)
{
    dynamicComponent = std::make_unique<DynamicTransformComponent>();
    staticComponent = std::make_unique<StaticTransformComponent>();
}

Transformation::~Transformation()
{
}

void Transformation::addDynamic(ITransformComponent* component)
{
    if (component)
    {
        dynamicComponent->add(component);
        useDynamic = true;
    }
}

void Transformation::addStatic(ITransformComponent* component)
{
    if (component)
    {
        staticComponent->add(component);
    }
}

glm::mat4 Transformation::getMatrix() const
{
    glm::mat4 result = glm::mat4(1.0f);
    result = result * dynamicComponent->getMatrix();
    result = result * staticComponent->getMatrix();
    return result;
}

void Transformation::update(float deltaTime)
{
    dynamicComponent->update(deltaTime);
    staticComponent->update(deltaTime);
}