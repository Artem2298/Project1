#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "CameraObserver.h"

class Camera
{
private:
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;

    float yaw;
    float pitch;

    float fov;
    float aspect;
    float near;
    float far;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    std::vector<CameraObserver*> observers;

    void updateViewMatrix();
    void notify();

public:
    Camera(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up,
        float fov, float aspect, float near, float far);
    ~Camera();

    void attach(CameraObserver* observer);
    void detach(CameraObserver* observer);

    void setPosition(const glm::vec3& newEye);
    void setTarget(const glm::vec3& newTarget);
    void setUp(const glm::vec3& newUp);
    void updateDirection(float deltaYaw, float deltaPitch);
    void setPerspective(float newFov, float newAspect, float newNear, float newFar);

    const glm::mat4& getCamera() const { return viewMatrix; }
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
    const glm::vec3& getEye() const { return eye; }
    const glm::vec3& getUp() const { return up; }
    const glm::vec3& getTarget() { return target; }

    float getFov() const { return fov; }
    float getAspect() const { return aspect; }
    float getNear() const { return near; }
    float getFar() const { return far; }
};
