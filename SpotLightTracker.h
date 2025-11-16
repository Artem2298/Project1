#pragma once
#include "CameraObserver.h"
#include "SpotLight.h"

class SpotLightTracker : public CameraObserver
{
private:
    SpotLight* spotlight;

public:
    explicit SpotLightTracker(SpotLight* light);
    ~SpotLightTracker() override = default;

    void onCameraChanged(Camera* camera) override;

    SpotLight* getSpotLight() const { return spotlight; }
};
