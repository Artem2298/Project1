#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include "LightObserver.h"

class ShaderProgram;

class Light
{
private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;

    std::vector<LightObserver*> observers;

    void notify();

public:
    Light(const glm::vec3& position = glm::vec3(0.0f, 10.0f, 0.0f),
        const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
        float intensity = 1.0f);
    ~Light();

    void attach(LightObserver* observer);
    void detach(LightObserver* observer);

    void setPosition(const glm::vec3& pos);
    void setColor(const glm::vec3& col);
    void setIntensity(float inten);
    void setAttenuation(float constant, float linear, float quadratic);

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getColor() const { return color; }
    float getIntensity() const { return intensity; }
    float getConstant() const { return constant; }
    float getLinear() const { return linear; }
    float getQuadratic() const { return quadratic; }

    void applyToShader(ShaderProgram& shader, int lightIndex = 0) const;
};