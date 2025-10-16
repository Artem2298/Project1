#pragma once

class Camera;

class CameraObserver
{
public:
    virtual ~CameraObserver() = default;
    virtual void onCameraChanged(Camera* camera) = 0;
};