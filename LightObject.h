#pragma once
#include "DrawableObject.h"
#include "Light.h"
#include "DynamicTranslateTransform.h"
#include <glm/vec3.hpp>

class LightObject : public DrawableObject
{
private:
    Light* attachedLight;
    DynamicTranslateTransform* dynamicTransform;

    glm::vec3 territoryCenter;
    float territoryRadius;
    float territoryMinHeight;
    float territoryMaxHeight;

    glm::vec3 currentPosition;
    glm::vec3 currentVelocity;
    float speed;

public:
    LightObject(glm::vec3 center,
        float radius,
        float minHeight,
        float maxHeight,
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 0.3f),
        float lightIntensity = 2.0f,
        float constant = 1.0f,
        float linear = 0.35f,
        float quadratic = 0.44f);

    ~LightObject();

    void update(float deltaTime) override;

    Light* getLight() const { return attachedLight; }
    void setSpeed(float s) { speed = s; }
    float getSpeed() const { return speed; }
    glm::vec3 getPosition() const { return currentPosition; }

private:
    bool isOutOfBounds(const glm::vec3& pos) const;
    glm::vec3 getNewRandomDirection() const;
    glm::vec3 clampToTerritory(const glm::vec3& pos) const;
    glm::vec3 getRandomStartPosition() const;
    float randomFloat(float min, float max) const;
};