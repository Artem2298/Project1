#include "Light.h"
#include "ShaderProgram.h"
#include <iostream>
#include <algorithm>

Light::Light(const glm::vec3& position, const glm::vec3& color, float intensity)
    : position(position),
    color(color),
    intensity(intensity),
    constant(1.0f),
    linear(0.09f),
    quadratic(0.032f)
{
    std::cout << "Light created at position: ("
        << position.x << ", " << position.y << ", " << position.z << ")\n";
}

Light::~Light()
{
    observers.clear();
    std::cout << "Light destroyed\n";
}

void Light::attach(LightObserver* observer)
{
    if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end())
    {
        observers.push_back(observer);
        std::cout << "Observer attached to light. Total observers: " << observers.size() << "\n";
    }
}

void Light::detach(LightObserver* observer)
{
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
    {
        observers.erase(it);
        std::cout << "Observer detached from light. Total observers: " << observers.size() << "\n";
    }
}

void Light::notify()
{
    std::cout << "[Observer Pattern] Light notifying " << observers.size() << " observer(s)...\n";
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
    std::cout << "Light position changed to: ("
        << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
    notify();
}

void Light::setColor(const glm::vec3& col)
{
    color = col;
    std::cout << "Light color changed to: ("
        << col.r << ", " << col.g << ", " << col.b << ")\n";
    notify();
}

void Light::setIntensity(float inten)
{
    intensity = inten;
    std::cout << "Light intensity changed to: " << inten << "\n";
    notify();
}

void Light::setAttenuation(float c, float l, float q)
{
    constant = c;
    linear = l;
    quadratic = q;
    std::cout << "Light attenuation changed\n";
    notify();
}

void Light::applyToShader(ShaderProgram& shader, int lightIndex) const
{
    if (lightIndex == 0)
    {
        shader.setUniform("lightPosition", position);
        shader.setUniform("lightColor", color);
        shader.setUniform("lightIntensity", intensity);
    }
    else
    {
        std::string posName = "lights[" + std::to_string(lightIndex) + "].position";
        std::string colorName = "lights[" + std::to_string(lightIndex) + "].color";
        std::string intensityName = "lights[" + std::to_string(lightIndex) + "].intensity";

        shader.setUniform(posName, position);
        shader.setUniform(colorName, color);
        shader.setUniform(intensityName, intensity);
    }
}