#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <algorithm>

Camera::Camera(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up,
               float fov, float aspect, float near, float far)
    : eye(eye), up(glm::normalize(up)),
      fov(fov), aspect(aspect), near(near), far(far)
{
    this->target = glm::normalize(target);
    
    yaw = glm::atan(this->target.z, this->target.x);
    float pitchValue = glm::clamp(this->target.y, -1.0f, 1.0f);
    pitch = glm::asin(pitchValue);

    updateViewMatrix();
    projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    
    std::cout << "Camera initialized\n";
}

Camera::~Camera() 
{
    observers.clear();
}

void Camera::updateViewMatrix() 
{
    viewMatrix = glm::lookAt(eye, eye + target, up);
}

void Camera::attach(CameraObserver* observer) 
{
    if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) 
    {
        observers.push_back(observer);
        std::cout << "Observer attached to camera. Total observers: " << observers.size() << "\n";
    }
}

void Camera::detach(CameraObserver* observer) 
{
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) 
    {
        observers.erase(it);
        std::cout << "Observer detached from camera. Total observers: " << observers.size() << "\n";
    }
}

void Camera::notify() 
{
    for (auto observer : observers) 
    {
        if (observer) 
        {
            observer->onCameraChanged(this);
        }
    }
}

void Camera::setPosition(const glm::vec3& newEye) 
{
    eye = newEye;
    updateViewMatrix();
    notify();
}

void Camera::setTarget(const glm::vec3& newTarget) 
{
    target = glm::normalize(newTarget);
    
    yaw = glm::atan(target.z, target.x);
    float pitchValue = glm::clamp(target.y, -1.0f, 1.0f);
    pitch = glm::asin(pitchValue);
    
    updateViewMatrix();
    notify();
}

void Camera::setUp(const glm::vec3& newUp) 
{
    up = glm::normalize(newUp);
    updateViewMatrix();
    notify();
}

void Camera::updateDirection(float deltaYaw, float deltaPitch) 
{
    yaw += deltaYaw;
    pitch += deltaPitch;

    const float maxPitch = glm::radians(89.0f);
    pitch = glm::clamp(pitch, -maxPitch, maxPitch);

    glm::vec3 newTarget = glm::vec3(
        glm::cos(pitch) * glm::cos(yaw),
        glm::sin(pitch),
        glm::cos(pitch) * glm::sin(yaw)
    );

    float length = glm::length(newTarget);
    if (length < 0.001f) 
    {
        std::cout << "WARNING: Invalid target direction\n";
        return;
    }

    target = newTarget / length;

    float dot = glm::dot(target, glm::normalize(up));
    if (std::abs(dot) > 0.95f) 
    {
        glm::vec3 worldUp = (std::abs(target.y) < 0.9f) 
            ? glm::vec3(0.0f, 1.0f, 0.0f) 
            : glm::vec3(1.0f, 0.0f, 0.0f);
        
        glm::vec3 right = glm::normalize(glm::cross(target, worldUp));
        up = glm::normalize(glm::cross(right, target));
    }

    updateViewMatrix();
    notify();
}

void Camera::setPerspective(float newFov, float newAspect, float newNear, float newFar) 
{
    fov = newFov;
    aspect = newAspect;
    near = newNear;
    far = newFar;
    projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    notify();
}