#include "LightObject.h"
#include <iostream>
#include <cmath>

LightObject::LightObject(glm::vec3 center,
    float radius,
    glm::vec3 lightColor,
    float lightIntensity,
    float constant,
    float linear,
    float quadratic)
    : DrawableObject(true)
    , orbitCenter(center)
    , orbitRadius(radius)
    , orbitSpeed(1.0f)
    , currentAngle(0.0f)
    , orbitHeight(1.5f)
    , dynamicTransform(nullptr)
    , attachedLight(nullptr)
{
    std::cout << "LightObject: Creating at orbit center ("
        << center.x << ", " << center.y << ", " << center.z
        << ") radius=" << radius << std::endl;

    attachedLight = new Light(center, lightColor, lightIntensity, constant, linear, quadratic);

    float x = orbitCenter.x + orbitRadius * std::cos(currentAngle);
    float y = orbitHeight;
    float z = orbitCenter.z + orbitRadius * std::sin(currentAngle);
    glm::vec3 startPosition(x, y, z);

    dynamicTransform = new DynamicTranslateTransform(startPosition, glm::vec3(0.0f));
    addDynamicTransform(dynamicTransform);

    attachedLight->setPosition(startPosition);

    std::cout << "LightObject: Initialization complete" << std::endl;
}

LightObject::~LightObject()
{
    std::cout << "LightObject: Destroying..." << std::endl;
    if (attachedLight != nullptr) {
        delete attachedLight;
        attachedLight = nullptr;
    }
}

void LightObject::update(float deltaTime)
{
    currentAngle += orbitSpeed * deltaTime;
    if (currentAngle > 2.0f * 3.14159265f) {
        currentAngle -= 2.0f * 3.14159265f;
    }

    float x = orbitCenter.x + orbitRadius * std::cos(currentAngle);
    float y = orbitHeight;
    float z = orbitCenter.z + orbitRadius * std::sin(currentAngle);
    glm::vec3 newPosition(x, y, z);

    dynamicTransform->setPosition(newPosition);

    dynamicTransform->setVelocity(glm::vec3(0.0f));

    DrawableObject::update(deltaTime);

    if (attachedLight != nullptr) {
        attachedLight->setPosition(newPosition);
    }

    static float debugTimer = 0.0f;
    debugTimer += deltaTime;
    if (debugTimer >= 2.0f) {
        glm::vec3 visualPos = dynamicTransform->getPosition();
        glm::vec3 lightPos = attachedLight->getPosition();

        std::cout << "\n===== FIREFLY DEBUG =====" << std::endl;
        std::cout << "Angle: " << currentAngle << " rad" << std::endl;
        std::cout << "Visual: (" << visualPos.x << ", " << visualPos.y << ", " << visualPos.z << ")" << std::endl;
        std::cout << "Light:  (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")" << std::endl;
        std::cout << "Distance: " << glm::length(visualPos - lightPos) << std::endl;
        std::cout << "=========================\n" << std::endl;

        debugTimer = 0.0f;
    }
}