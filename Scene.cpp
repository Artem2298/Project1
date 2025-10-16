#include "Scene.h"
#include <iostream>
#include <algorithm>

Scene::Scene()
    : viewMatrix(glm::mat4(1.0f)),
    projectionMatrix(glm::mat4(1.0f))
{
    std::cout << "Scene created\n";
}

Scene::~Scene()
{
    if (camera)
    {
        camera->detach(this);
    }
    clear();
    std::cout << "Scene destroyed\n";
}

void Scene::addObject(DrawableObject* obj)
{
    if (obj == nullptr)
    {
        std::cerr << "ERROR: Cannot add nullptr to scene\n";
        return;
    }
    objects.push_back(std::unique_ptr<DrawableObject>(obj));
}

void Scene::removeObject(DrawableObject* obj)
{
    if (obj == nullptr)
        return;

    auto it = std::find_if(objects.begin(), objects.end(),
        [obj](const std::unique_ptr<DrawableObject>& ptr) {
            return ptr.get() == obj;
        });

    if (it != objects.end())
    {
        objects.erase(it);
    }
}

void Scene::clear()
{
    objects.clear();
    std::cout << "Scene cleared\n";
}

void Scene::update(float deltaTime)
{
    for (auto& obj : objects)
    {
        if (obj)
            obj->update(deltaTime);
    }
}

void Scene::render(ShaderProgram& shader)
{
    shader.use();
    shader.setUniform("viewMatrix", viewMatrix);
    shader.setUniform("projectionMatrix", projectionMatrix);

    for (auto& obj : objects)
    {
        if (obj)
            obj->draw(shader);
    }
}

void Scene::setCamera(Camera* newCamera)
{
    if (camera)
    {
        camera->detach(this);
    }

    camera.reset(newCamera);

    if (camera)
    {
        camera->attach(this);

        viewMatrix = camera->getCamera();
        projectionMatrix = camera->getProjectionMatrix();

    }
}

void Scene::updateCameraMatrices()
{
    if (camera)
    {
        viewMatrix = camera->getCamera();
        projectionMatrix = camera->getProjectionMatrix();
    }
}

void Scene::onCameraChanged(Camera* cam)
{
    if (!cam) return;

    viewMatrix = cam->getCamera();
    projectionMatrix = cam->getProjectionMatrix();

}

DrawableObject* Scene::getObject(size_t index)
{
    if (index >= objects.size())
    {
        std::cerr << "ERROR: Object index out of bounds\n";
        return nullptr;
    }
    return objects[index].get();
}

const DrawableObject* Scene::getObject(size_t index) const
{
    if (index >= objects.size())
    {
        std::cerr << "ERROR: Object index out of bounds\n";
        return nullptr;
    }
    return objects[index].get();
}