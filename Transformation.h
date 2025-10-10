#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

class Transformation
{
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scaleValue;

    glm::mat4 matrix;
    bool isDirty;

    void updateMatrix();     

public:
    Transformation();
    ~Transformation();

    void setPosition(const glm::vec3& pos);
    void translate(const glm::vec3& offset);
    glm::vec3 getPosition() const { return position; }

    void setRotation(const glm::vec3& rot);
    void rotate(const glm::vec3& axis, float angle);
    glm::vec3 getRotation() const { return rotation; }

    void setScale(const glm::vec3& s);
    void applyScale(const glm::vec3& s);
    glm::vec3 getScale() const { return scaleValue; }

    glm::mat4 getMatrix();

    void reset();

    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;
};