#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <vector>
#include <memory>

class DrawableObject;
class Camera;
class Light;
class SpotLight;
class ShaderProgram;

class SceneRenderer
{
private:
    bool useStencilTest;

    void setupStencilTest();
    void disableStencilTest();
    void renderObject(DrawableObject* obj,
        const glm::mat4& viewMatrix,
        const glm::mat4& projectionMatrix,
        Camera* camera,
        const std::vector<Light*>& lights,
        SpotLight* spotlight);

    void applyMatrices(ShaderProgram* shader,
        const glm::mat4& modelMatrix,
        const glm::mat4& viewMatrix,
        const glm::mat4& projectionMatrix);

    void applyCamera(ShaderProgram* shader, Camera* camera);

    void applyLights(ShaderProgram* shader,
        const std::vector<Light*>& lights,
        SpotLight* spotlight);

    void applyMaterial(ShaderProgram* shader, DrawableObject* obj);

    void applyTexture(ShaderProgram* shader, DrawableObject* obj);

public:
    SceneRenderer();
    ~SceneRenderer();

    void render(const std::vector<std::unique_ptr<DrawableObject>>& objects,
        const glm::mat4& viewMatrix,
        const glm::mat4& projectionMatrix,
        Camera* camera,
        const std::vector<Light*>& lights,
        SpotLight* spotlight);

    void setUseStencilTest(bool use) { useStencilTest = use; }
    bool isUsingStencilTest() const { return useStencilTest; }
};