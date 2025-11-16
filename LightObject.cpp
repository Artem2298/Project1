#include "LightObject.h"
#include <iostream>
#include <cmath>
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

LightObject::LightObject(glm::vec3 center,
    float radius,
    float minHeight,
    float maxHeight,
    glm::vec3 lightColor,
    float lightIntensity,
    float constant,
    float linear,
    float quadratic)
    : DrawableObject(true)
    , territoryCenter(center)
    , territoryRadius(radius)
    , territoryMinHeight(minHeight)
    , territoryMaxHeight(maxHeight)
    , speed(2.0f) 
    , dynamicTransform(nullptr)
    , attachedLight(nullptr)
{

    attachedLight = new Light(
        center,
        lightColor,
        lightIntensity,
        constant,
        linear,
        quadratic
    );

    currentPosition = getRandomStartPosition();

    currentVelocity = getNewRandomDirection() * speed;

    dynamicTransform = new DynamicTranslateTransform(
        currentPosition,
        glm::vec3(0.0f)
    );

    addDynamicTransform(dynamicTransform);

    attachedLight->setPosition(currentPosition);

}

LightObject::~LightObject()
{
    if (attachedLight != nullptr) {
        delete attachedLight;
        attachedLight = nullptr;
    }
}

void LightObject::update(float deltaTime)
{
    glm::vec3 newPosition = currentPosition + currentVelocity * deltaTime;

    if (isOutOfBounds(newPosition)) {

        currentVelocity = getNewRandomDirection() * speed;

        newPosition = clampToTerritory(newPosition);
    }

    currentPosition = newPosition;

    dynamicTransform->setPosition(currentPosition);
    dynamicTransform->setVelocity(glm::vec3(0.0f));

    DrawableObject::update(deltaTime);

    if (attachedLight != nullptr) {
        attachedLight->setPosition(currentPosition);
    }
}

bool LightObject::isOutOfBounds(const glm::vec3& pos) const
{
    float distXZ = glm::length(glm::vec2(pos.x - territoryCenter.x,
        pos.z - territoryCenter.z));
    if (distXZ > territoryRadius) {
        return true;
    }

    if (pos.y < territoryMinHeight || pos.y > territoryMaxHeight) {
        return true;
    }

    return false;
}

glm::vec3 LightObject::getNewRandomDirection() const
{
    float theta = randomFloat(0.0f, 2.0f * 3.14159265f);
    float phi = randomFloat(-0.3f, 0.3f);

    glm::vec3 direction(
        std::cos(phi) * std::cos(theta),
        std::sin(phi),                     
        std::cos(phi) * std::sin(theta)
    );

    return glm::normalize(direction);
}

glm::vec3 LightObject::clampToTerritory(const glm::vec3& pos) const
{
    glm::vec3 result = pos;

    float distXZ = glm::length(glm::vec2(pos.x - territoryCenter.x,
        pos.z - territoryCenter.z));
    if (distXZ > territoryRadius) {
        glm::vec2 direction = glm::normalize(glm::vec2(pos.x - territoryCenter.x,
            pos.z - territoryCenter.z));
        result.x = territoryCenter.x + direction.x * (territoryRadius - 0.1f);
        result.z = territoryCenter.z + direction.y * (territoryRadius - 0.1f);
    }

    result.y = glm::clamp(result.y, territoryMinHeight + 0.1f, territoryMaxHeight - 0.1f);

    return result;
}

glm::vec3 LightObject::getRandomStartPosition() const
{
    float angle = randomFloat(0.0f, 2.0f * 3.14159265f);
    float radius = randomFloat(0.0f, territoryRadius * 0.8f);
    float height = randomFloat(territoryMinHeight, territoryMaxHeight);

    return glm::vec3(
        territoryCenter.x + radius * std::cos(angle),
        height,
        territoryCenter.z + radius * std::sin(angle)
    );
}

float LightObject::randomFloat(float min, float max) const
{
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}