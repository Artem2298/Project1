#pragma once
#include "DrawableObject.h"

class Square : public DrawableObject
{
public:
    Square();
    ~Square();

    void update(float deltaTime) override;
};