#include "Scene.h"
#include "LightObject.h"
#include <algorithm>
#include <iostream>
#include "Texture.h"

Scene::Scene()
    : viewMatrix(glm::mat4(1.0f)),
    projectionMatrix(glm::mat4(1.0f)),
    spotlight(nullptr)
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

    std::cout << "Light added to scene. Total lights: " << lights.size() << std::endl;
}

void Scene::addLightObject(LightObject* lightObj)
{
    if (lightObj == nullptr) {
        std::cerr << "Scene::addLightObject() - lightObj is nullptr!" << std::endl;
        return;
    }

    std::cout << "Scene::addLightObject() - Adding LightObject..." << std::endl;

    objects.push_back(std::unique_ptr<DrawableObject>(lightObj));

    Light* light = lightObj->getLight();
    if (light != nullptr) {
        lights.push_back(light);

        light->attach(this);

        std::cout << "Scene::addLightObject() - Light added at ("
            << light->getPosition().x << ", "
            << light->getPosition().y << ", "
            << light->getPosition().z << ")"
            << " Total lights: " << lights.size() << std::endl;
    }
    else {
        std::cerr << "WARNING: LightObject has no attached light!" << std::endl;
    }
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

        glm::mat4 modelMatrix = obj->getModelMatrix();
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

        shader->setUniform("modelMatrix", modelMatrix);
        shader->setUniform("normalMatrix", normalMatrix);
        shader->setUniform("viewMatrix", viewMatrix);
        shader->setUniform("projectionMatrix", projectionMatrix);

        if (camera) {
            GLint loc = shader->getUniformLocation("cameraPosition");
            if (loc != -1) {
                shader->setUniform("cameraPosition", camera->getEye());
            }
        }

        int numLights = static_cast<int>(lights.size());
        if (numLights > 20) {
            numLights = 20;
        }

        GLint numLightsLoc = shader->getUniformLocation("numLights");
        if (numLightsLoc != -1) {
            shader->setUniform("numLights", numLights);
        }

        for (int i = 0; i < numLights; i++) {
            if (lights[i] != nullptr) {
                lights[i]->applyToShader(*shader, i);
            }
        }

        if (spotlight) {
            spotlight->applyToShader(*shader, "spotlight");
        }
        else {
            GLint spotlightEnabledLoc = shader->getUniformLocation("spotlight.enabled");
            if (spotlightEnabledLoc != -1) {
                shader->setUniform("spotlight.enabled", 0);
            }
        }

        GLint objectColorLoc = shader->getUniformLocation("objectColor");
        if (objectColorLoc != -1) {
            shader->setUniform("objectColor", obj->getObjectColor());
        }

        GLint shininessLoc = shader->getUniformLocation("shininess");
        if (shininessLoc != -1) {
            shader->setUniform("shininess", obj->getShininess());
        }

        Texture* texture = obj->getTexture();
        if (texture != nullptr && texture->isTextureLoaded()) {
            texture->bind(0);

            GLint textureLoc = shader->getUniformLocation("textureUnitID");
            if (textureLoc != -1) {
                shader->setUniform("textureUnitID", 0);
            }

            GLint useTextureLoc = shader->getUniformLocation("useTexture");
            if (useTextureLoc != -1) {
                shader->setUniform("useTexture", 1);
            }
        }
        else {
            GLint useTextureLoc = shader->getUniformLocation("useTexture");
            if (useTextureLoc != -1) {
                shader->setUniform("useTexture", 0);
            }
        }

        obj->draw();

        if (texture != nullptr && texture->isTextureLoaded()) {
            texture->unbind();
        }

        shader->unuse();
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
    std::cout << "? SpotLight added to scene" << std::endl;
}