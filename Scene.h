#pragma once
#include <vector>
#include <memory>
#include "DrawableObject.h"
#include "ShaderProgram.h"
#include <glm/mat4x4.hpp>

class Scene
{
private:
    std::vector<std::unique_ptr<DrawableObject>> objects;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* obj);

    void removeObject(DrawableObject* obj);

    void clear();

    size_t getObjectCount() const { return objects.size(); }

    void update(float deltaTime);

    void render(ShaderProgram& shader);

    void setViewMatrix(const glm::mat4& view) { viewMatrix = view; }
    void setProjectionMatrix(const glm::mat4& projection) { projectionMatrix = projection; }

    glm::mat4 getViewMatrix() const { return viewMatrix; }
    glm::mat4 getProjectionMatrix() const { return projectionMatrix; }

    DrawableObject* getObject(size_t index);
    const DrawableObject* getObject(size_t index) const;
};