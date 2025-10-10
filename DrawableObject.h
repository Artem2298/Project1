#pragma once
#include "Model.h"
#include "Transformation.h"
#include "ShaderProgram.h"
#include <glm/vec3.hpp>

class DrawableObject
{
protected:
    Model model;
    Transformation transform;
    glm::vec3 color;

public:
    DrawableObject();
    virtual ~DrawableObject();

    virtual void update(float deltaTime);
    virtual void draw(ShaderProgram& shader);

    void setPosition(const glm::vec3& pos) { transform.setPosition(pos); }
    void translate(const glm::vec3& offset) { transform.translate(offset); }
    glm::vec3 getPosition() const { return transform.getPosition(); }

    void setRotation(const glm::vec3& rot) { transform.setRotation(rot); }
    void rotate(const glm::vec3& axis, float angle) { transform.rotate(axis, angle); }
    glm::vec3 getRotation() const { return transform.getRotation(); }

    void setScale(const glm::vec3& s) { transform.setScale(s); }
    void applyScale(const glm::vec3& s) { transform.applyScale(s); }
    glm::vec3 getScale() const { return transform.getScale(); }

    void setColor(const glm::vec3& c) { color = c; }
    glm::vec3 getColor() const { return color; }

    Model& getModel() { return model; }
    Transformation& getTransformation() { return transform; }
};