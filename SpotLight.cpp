#include "SpotLight.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

SpotLight::SpotLight(const glm::vec3& position,
    const glm::vec3& direction,
    const glm::vec3& color,
    float intensity,
    float cutOff,
    float outerCutOff,
    float constant,
    float linear,
    float quadratic)
    : position(position)
    , direction(glm::normalize(direction))
    , color(color)
    , intensity(intensity)
    , cutOff(cutOff)
    , outerCutOff(outerCutOff)
    , constant(constant)
    , linear(linear)
    , quadratic(quadratic)
    , enabled(true)
{
    std::cout << "\nSpotLight created at position: ("
        << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}

void SpotLight::setPosition(const glm::vec3& pos)
{
    position = pos;
}

void SpotLight::setDirection(const glm::vec3& dir)
{
    direction = glm::normalize(dir);
}

void SpotLight::setColor(const glm::vec3& col)
{
    color = col;
}

void SpotLight::setIntensity(float inten)
{
    intensity = inten;
}

void SpotLight::setCutOff(float cutOffAngle, float outerCutOffAngle)
{
    cutOff = cutOffAngle;
    outerCutOff = outerCutOffAngle;
}

void SpotLight::setAttenuation(float c, float l, float q)
{
    constant = c;
    linear = l;
    quadratic = q;
}

void SpotLight::setEnabled(bool isEnabled)
{
    enabled = isEnabled;
}

void SpotLight::applyToShader(ShaderProgram& shader, const std::string& uniformName) const
{
    shader.setUniform(uniformName + ".position", position);
    shader.setUniform(uniformName + ".direction", direction);
    shader.setUniform(uniformName + ".color", color);
    shader.setUniform(uniformName + ".intensity", intensity);
    shader.setUniform(uniformName + ".cutOff", glm::cos(glm::radians(cutOff)));
    shader.setUniform(uniformName + ".outerCutOff", glm::cos(glm::radians(outerCutOff)));
    shader.setUniform(uniformName + ".constant", constant);
    shader.setUniform(uniformName + ".linear", linear);
    shader.setUniform(uniformName + ".quadratic", quadratic);
    shader.setUniform(uniformName + ".enabled", enabled ? 1 : 0);
}