#include "Light.h"
#include "ShaderProgram.h"
#include <iostream>
#include <algorithm>

Light::Light(const glm::vec3& position,
    const glm::vec3& color,
    float intensity,
    float constant,
    float linear,
    float quadratic)
    : position(position)
    , color(color)
    , intensity(intensity)
    , constant(constant)
    , linear(linear)
    , quadratic(quadratic)
{
}

Light::~Light()
{
    notifyDestruction();
}

void Light::notifyDestruction()
{
    std::vector<LightObserver*> observersCopy = observers;

    for (auto* observer : observersCopy) {
        if (observer != nullptr) {
            observer->onLightDestroyed(this);
        }
    }

    observers.clear();
}

void Light::attach(LightObserver* observer)
{
    if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end())
    {
        observers.push_back(observer);
    }
}

void Light::detach(LightObserver* observer)
{
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
    {
        observers.erase(it);
    }
}

void Light::notify()
{
    for (auto observer : observers)
    {
        if (observer)
        {
            observer->onLightChanged(this);
        }
    }
}

void Light::setPosition(const glm::vec3& pos)
{
    position = pos;
    notify();
}

void Light::setColor(const glm::vec3& col)
{
    color = col;
    notify();
}

void Light::setIntensity(float inten)
{
    intensity = inten;
    notify();
}

void Light::setAttenuation(float c, float l, float q)
{
    constant = c;
    linear = l;
    quadratic = q;
    notify();
}

void Light::applyToShader(ShaderProgram& shader, int lightIndex) const
{
    std::string base = "lights[" + std::to_string(lightIndex) + "].";

    shader.setUniform(base + "position", position);
    shader.setUniform(base + "color", color);
    shader.setUniform(base + "intensity", intensity);
    shader.setUniform(base + "constant", constant);
    shader.setUniform(base + "linear", linear);
    shader.setUniform(base + "quadratic", quadratic);
}