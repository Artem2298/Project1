#pragma once
#include "ITransformComponent.h"
#include "DynamicTransformComponent.h"
#include "StaticTransformComponent.h"
#include <memory>

class Transformation
{
private:
    std::unique_ptr<DynamicTransformComponent> dynamicComponent;
    std::unique_ptr<StaticTransformComponent> staticComponent;
    bool useDynamic;

public:
    Transformation(bool isDynamic = false);
    ~Transformation();

    void addDynamic(ITransformComponent* component);
    void addStatic(ITransformComponent* component);

    glm::mat4 getMatrix() const;
    void update(float deltaTime);

    void setDynamic(bool dynamic) { useDynamic = dynamic; }
    bool isDynamic() const { return useDynamic; }
};
