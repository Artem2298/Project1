#include "SpotLightTracker.h"
#include "Camera.h"
#include <iostream>

SpotLightTracker::SpotLightTracker(SpotLight* light)
    : spotlight(light)
{
    if (!spotlight) {
        std::cerr << "?? Warning: SpotLightTracker created with null spotlight!" << std::endl;
    }
    else {
        std::cout << "? SpotLightTracker initialized" << std::endl;
    }
}

void SpotLightTracker::onCameraChanged(Camera* camera)
{
    if (!spotlight || !camera) {
        return;
    }

    glm::vec3 pos = camera->getEye();
    glm::vec3 dir = camera->getTarget();

    /*dir.y = 0.0f;
    dir = glm::normalize(dir);*/


    spotlight->setPosition(pos);
    spotlight->setDirection(dir);

    glm::vec3 spotDir = spotlight->getDirection();

}