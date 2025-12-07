#include "Scene.h"
#include "LightObject.h"
#include <algorithm>
#include <iostream>
#include "Texture.h"

Scene::Scene()
    : viewMatrix(glm::mat4(1.0f)),
    projectionMatrix(glm::mat4(1.0f)),
    spotlight(nullptr),
    nextObjectID(1)
{
}

Scene::~Scene()
{
    if (camera)
    {
        camera->detach(this);
    }

    for (auto light : lights)
    {
        if (light)
        {
            light->detach(this);
        }
    }

    clear();
}

void Scene::addLight(Light* light)
{
    if (light == nullptr)
    {
        std::cerr << "ERROR: Cannot add nullptr light to scene\n";
        return;
    }

    lights.push_back(light);
    light->attach(this);

    std::cout << "\nLight added to scene. Total lights: " << lights.size() << std::endl;
}

void Scene::addLightObject(LightObject* lightObj)
{
    if (lightObj == nullptr) {
        std::cerr << "Scene::addLightObject() - lightObj is nullptr!" << std::endl;
        return;
    }

    objects.push_back(std::unique_ptr<DrawableObject>(lightObj));

    Light* light = lightObj->getLight();
    if (light != nullptr) {
        lights.push_back(light);

        light->attach(this);
    }
    else {
        std::cerr << "WARNING: LightObject has no attached light!" << std::endl;
    }
}

void Scene::addObject(DrawableObject* obj)
{
    if (obj == nullptr)
    {
        std::cerr << "ERROR: Cannot add nullptr to scene\n";
        return;
    }

    obj->setID(nextObjectID);
    std::cout << "Object added with ID: " << nextObjectID << std::endl;
    nextObjectID++;

    objects.push_back(std::unique_ptr<DrawableObject>(obj));
}

void Scene::clear()
{
    objects.clear();
}

void Scene::update(float deltaTime)
{
    if (camera)
    {
        projectionMatrix = camera->getProjectionMatrix();
        viewMatrix = camera->getViewMatrix();
    }

    for (auto& obj : objects)
    {
        if (obj)
            obj->update(deltaTime);
    }
}

void Scene::render()
{
    renderer.render(objects, viewMatrix, projectionMatrix, camera.get(), lights, spotlight);
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

void Scene::onCameraChanged(Camera* camera)
{
    if (camera) {
        viewMatrix = camera->getCamera();
    }
}

void Scene::onLightChanged(Light* light)
{
    if (!light) return;
}

void Scene::onLightDestroyed(Light* light)
{
    std::cout << "Scene::onLightDestroyed() - Removing light from scene..." << std::endl;

    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end()) {
        lights.erase(it);
        std::cout << "Scene::onLightDestroyed() - Light removed. Total lights: "
            << lights.size() << std::endl;
    }
    else {
        std::cerr << "WARNING: Light not found in scene!" << std::endl;
    }
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

ShaderProgram* Scene::createShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    auto shader = std::make_unique<ShaderProgram>();

    if (!shader->loadFromFiles(vertexPath, fragmentPath)) {
        std::cerr << "Scene: Failed to load shader: " << vertexPath << " + " << fragmentPath << std::endl;
        return nullptr;
    }

    std::cout << "Scene: Loaded shader: " << vertexPath << " + " << fragmentPath << std::endl;

    shaders.push_back(std::move(shader));
    return shaders.back().get();
}

void Scene::setSpotLight(SpotLight* light)
{
    spotlight = light;
    std::cout << "SpotLight added to scene" << std::endl;
}

DrawableObject* Scene::findObjectByID(int id)
{
    for (auto& obj : objects)
    {
        if (obj->getID() == id)
        {
            return obj.get();
        }
    }
    return nullptr;
}

void Scene::putTree(const glm::vec3& position)
{
    DrawableObject* tree = new DrawableObject();

    if (!shaders.empty()) {
        tree->setShader(shaders[0].get());
    }
    else {
        std::cerr << "No shaders available in scene!" << std::endl;
        delete tree;
        return;
    }

    if (!tree->loadModel("models/tree.h", "tree")) {
        std::cerr << "Failed to load tree model!" << std::endl;
        delete tree;
        return;
    }

    tree->setObjectColor(glm::vec3(0.3f, 0.2f, 0.1f));
    tree->setShininess(32.0f);

    tree->addStaticTransform(new TranslateTransform(position));

    addObject(tree);
}
