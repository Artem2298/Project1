#include "SceneRenderer.h"
#include "DrawableObject.h"
#include "Camera.h"
#include "Light.h"
#include "SpotLight.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>

SceneRenderer::SceneRenderer()
    : useStencilTest(true)
{
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::render(
    const std::vector<std::unique_ptr<DrawableObject>>& objects,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix,
    Camera* camera,
    const std::vector<Light*>& lights,
    SpotLight* spotlight)
{
    if (useStencilTest) {
        setupStencilTest();
    }

    for (const auto& obj : objects) {
        if (obj) {
            renderObject(obj.get(), viewMatrix, projectionMatrix, camera, lights, spotlight);
        }
    }

    if (useStencilTest) {
        disableStencilTest();
    }
}

void SceneRenderer::setupStencilTest()
{
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void SceneRenderer::disableStencilTest()
{
    glDisable(GL_STENCIL_TEST);
}

void SceneRenderer::renderObject(
    DrawableObject* obj,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix,
    Camera* camera,
    const std::vector<Light*>& lights,
    SpotLight* spotlight)
{
    if (obj->getShader() == nullptr) {
        std::cerr << "SceneRenderer::renderObject() - Object has no shader!" << std::endl;
        return;
    }

    ShaderProgram* shader = obj->getShader();
    shader->use();

    if (useStencilTest) {
        glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
    }

    glm::mat4 modelMatrix = obj->getModelMatrix();
    applyMatrices(shader, modelMatrix, viewMatrix, projectionMatrix);

    applyCamera(shader, camera);

    applyLights(shader, lights, spotlight);

    applyMaterial(shader, obj);

    applyTexture(shader, obj);

    obj->draw();

    Texture* texture = obj->getTexture();
    if (texture != nullptr && texture->isTextureLoaded()) {
        texture->unbind();
    }

    shader->unuse();
}

void SceneRenderer::applyMatrices(
    ShaderProgram* shader,
    const glm::mat4& modelMatrix,
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix)
{
    shader->setUniform("modelMatrix", modelMatrix);

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    shader->setUniform("normalMatrix", normalMatrix);

    shader->setUniform("viewMatrix", viewMatrix);

    shader->setUniform("projectionMatrix", projectionMatrix);
}

void SceneRenderer::applyCamera(ShaderProgram* shader, Camera* camera)
{
    if (camera) {
        GLint loc = shader->getUniformLocation("cameraPosition");
        if (loc != -1) {
            shader->setUniform("cameraPosition", camera->getEye());
        }
    }
}

void SceneRenderer::applyLights(
    ShaderProgram* shader,
    const std::vector<Light*>& lights,
    SpotLight* spotlight)
{
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
}

void SceneRenderer::applyMaterial(ShaderProgram* shader, DrawableObject* obj)
{
    GLint objectColorLoc = shader->getUniformLocation("objectColor");
    if (objectColorLoc != -1) {
        shader->setUniform("objectColor", obj->getObjectColor());
    }

    GLint shininessLoc = shader->getUniformLocation("shininess");
    if (shininessLoc != -1) {
        shader->setUniform("shininess", obj->getShininess());
    }
}

void SceneRenderer::applyTexture(ShaderProgram* shader, DrawableObject* obj)
{
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
}