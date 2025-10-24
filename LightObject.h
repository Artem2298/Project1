#pragma once
#include "DrawableObject.h"
#include "Light.h"
#include "DynamicTranslateTransform.h"
#include <glm/vec3.hpp>

class LightObject : public DrawableObject
{
private:
    Light* attachedLight;
    glm::vec3 orbitCenter;
    float orbitRadius;
    float orbitSpeed;
    float currentAngle;
    float orbitHeight;
    DynamicTranslateTransform* dynamicTransform;

public:
    LightObject(glm::vec3 center,
        float radius,
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 0.3f),
        float lightIntensity = 1.0f,
        float constant = 1.0f,
        float linear = 0.7f,
        float quadratic = 1.8f);

    ~LightObject();

    void update(float deltaTime) override;

    Light* getAttachedLight() { return attachedLight; }
    const Light* getAttachedLight() const { return attachedLight; }

    void setOrbitSpeed(float speed) { orbitSpeed = speed; }
    void setOrbitHeight(float height) { orbitHeight = height; }
};