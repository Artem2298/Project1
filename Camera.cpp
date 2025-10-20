#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <algorithm>

Camera::Camera(const glm::vec3& eye, const glm::vec3& targetPoint, const glm::vec3& up,
    float fov, float aspect, float near, float far)
    : eye(eye),
    up(up),
    fov(fov),
    aspect(aspect),
    near(near),
    far(far)
{
    target = glm::normalize(targetPoint - eye);

    yaw = glm::degrees(atan2(target.z, target.x));
    pitch = glm::degrees(asin(target.y));

    updateViewMatrix();
    projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);

    std::cout << "Camera initialized at position: ("
        << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
    std::cout << "Camera direction: ("
        << target.x << ", " << target.y << ", " << target.z << ")" << std::endl;
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
    for (auto observer : observers) {
        observer->onCameraChanged(this);
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
    target = glm::normalize(newTarget - eye);
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
    yaw -= deltaYaw;
    pitch -= deltaPitch;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    target = glm::normalize(direction);

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

const glm::vec3& Camera::getDirection()
{
    return target;
}