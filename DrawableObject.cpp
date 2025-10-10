#include "DrawableObject.h"

DrawableObject::DrawableObject()
    : color(1.0f, 1.0f, 1.0f)
{
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::update(float deltaTime)
{

}

void DrawableObject::draw(ShaderProgram& shader)
{
    shader.use();

    shader.setUniformMatrix4f("model", transform.getMatrix());
    shader.setUniform3f("color", color.x, color.y, color.z);

    model.draw();
}