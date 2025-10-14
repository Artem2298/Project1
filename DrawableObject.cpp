#include "DrawableObject.h"

DrawableObject::DrawableObject(bool isDynamic)
    : transform(isDynamic)
{
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::update(float deltaTime)
{
    transform.update(deltaTime);
}

void DrawableObject::draw(ShaderProgram& shader)
{
    shader.use();
    shader.setUniform("model", transform.getMatrix());
    model.draw();
}

void DrawableObject::addStaticTransform(ITransformComponent* component)
{
    transform.addStatic(component);
}

void DrawableObject::addDynamicTransform(ITransformComponent* component)
{
    transform.addDynamic(component);
}