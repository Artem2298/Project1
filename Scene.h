#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/mat4x4.hpp>
#include "DrawableObject.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "CameraObserver.h"
#include "Light.h"
#include "LightObserver.h"
#include "SpotLight.h"

class LightObject;

class Scene : public CameraObserver, public LightObserver
{
private:
    std::vector<std::unique_ptr<DrawableObject>> objects;
    std::unique_ptr<Camera> camera;
    std::vector<Light*> lights;
    SpotLight* spotlight;

    std::vector<std::unique_ptr<ShaderProgram>> shaders;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* obj);
    void addLightObject(LightObject* lightObj);
    void removeObject(DrawableObject* obj);

    void clear();
    void update(float deltaTime);
    void render();

    void setCamera(Camera* newCamera);
    void updateCameraMatrices();

    void addLight(Light* light);
    void removeLight(Light* light);
    const std::vector<Light*>& getLights() const { return lights; }
    size_t getLightCount() const { return lights.size(); }

    void onCameraChanged(Camera* camera) override;
    void onLightChanged(Light* light) override;
    void onLightDestroyed(Light* light) override;

    ShaderProgram* createShader(const std::string& vertexPath, const std::string& fragmentPath);

    void setSpotLight(SpotLight* light);

    Camera* getCamera() const { return camera.get(); }
    size_t getObjectCount() const { return objects.size(); }
    DrawableObject* getObject(size_t index);
    const DrawableObject* getObject(size_t index) const;

    void setProjectionMatrix(const glm::mat4& proj) { projectionMatrix = proj; }
    const glm::mat4& getViewMatrix() const { return viewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
};