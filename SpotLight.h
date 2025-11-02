#pragma once
#include <glm/vec3.hpp>
#include <string>

class ShaderProgram;

class SpotLight
{
private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;

    float intensity;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    bool enabled;

public:
    SpotLight(const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& direction = glm::vec3(0.0f, 0.0f, -1.0f),
        const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
        float intensity = 1.0f,
        float cutOff = 12.5f,
        float outerCutOff = 17.5f,
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f);

    ~SpotLight() = default;

    void setPosition(const glm::vec3& pos);
    void setDirection(const glm::vec3& dir);
    void setColor(const glm::vec3& col);
    void setIntensity(float inten);
    void setCutOff(float cutOffAngle, float outerCutOffAngle);
    void setAttenuation(float c, float l, float q);
    void setEnabled(bool isEnabled);

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getDirection() const { return direction; }
    const glm::vec3& getColor() const { return color; }
    float getIntensity() const { return intensity; }
    float getCutOff() const { return cutOff; }
    float getOuterCutOff() const { return outerCutOff; }
    float getConstant() const { return constant; }
    float getLinear() const { return linear; }
    float getQuadratic() const { return quadratic; }
    bool isEnabled() const { return enabled; }

    void applyToShader(ShaderProgram& shader, const std::string& uniformName = "spotlight") const;
};