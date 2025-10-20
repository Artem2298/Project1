#pragma once

class Light;

class LightObserver
{
public:
    virtual ~LightObserver() = default;
    virtual void onLightChanged(Light* light) = 0;
};