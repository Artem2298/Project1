#include "Scene.h"
#include <iostream>

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

    for (auto light : lights)
    {
        if (light)
        {
            light->detach(this);
        }
    }

    clear();
    std::cout << "Scene destroyed\n";
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

    std::cout << "Light added to scene. Total lights: " << lights.size() << "\n";
}

void Scene::removeLight(Light* light)
{
    if (light == nullptr)
        return;

    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end())
    {
        (*it)->detach(this);
        lights.erase(it);
        std::cout << "Light removed from scene. Total lights: " << lights.size() << "\n";
    }
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

void Scene::render()
{
    for (auto& obj : objects) {
        if (obj->getShader() == nullptr) {
            std::cerr << "Scene::render() - Object has no shader!" << std::endl;
            continue;
        }

        ShaderProgram* shader = obj->getShader();
        shader->use();

        shader->setUniform("viewMatrix", viewMatrix);
        shader->setUniform("projectionMatrix", projectionMatrix);

        if (camera) {
            GLint loc = shader->getUniformLocation("cameraPosition");
            if (loc != -1) {
                shader->setUniform("cameraPosition", camera->getEye());
            }
        }

        if (!lights.empty() && lights[0] != nullptr) {
            Light* light = lights[0];

            if (shader->getUniformLocation("lightPosition") != -1) {
                shader->setUniform("lightPosition", light->getPosition());
            }
            if (shader->getUniformLocation("lightColor") != -1) {
                shader->setUniform("lightColor", light->getColor());
            }
            if (shader->getUniformLocation("lightIntensity") != -1) {
                shader->setUniform("lightIntensity", light->getIntensity());
            }
        }

        obj->draw();
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

void Scene::onCameraChanged(Camera* camera)
{
    if (camera) {
        viewMatrix = camera->getCamera();
    }
}

void Scene::onLightChanged(Light* light)
{
    if (!light) return;

    std::cout << "  ? [Scene Observer] Received light update notification\n";
    std::cout << "  ? [Scene Observer] Light parameters updated in scene ?\n";
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