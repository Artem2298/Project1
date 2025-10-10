#pragma once
#include "DrawableObject.h"

class Triangle : public DrawableObject
{
private:
    float rotationSpeed;

public:
    Triangle();
    ~Triangle();

    void update(float deltaTime) override;
    void setRotationSpeed(float speed) { rotationSpeed = speed; }
};