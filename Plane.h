#pragma once
#include "DrawableObject.h"

class Plane : public DrawableObject
{
public:
    Plane();
    ~Plane();
    void update(float deltaTime) override;
};