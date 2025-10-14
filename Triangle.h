#pragma once
#include "DrawableObject.h"
#include "DynamicRotateTransform.h"
#include "TranslateTransform.h"

class Triangle : public DrawableObject
{
private:
    DynamicRotateTransform* rotateTransform;

public:
    Triangle();
    ~Triangle();
    void update(float deltaTime) override;
    void setRotationSpeed(float speed);
};