#include "Transformation.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

Transformation::Transformation()
    : position(0.0f, 0.0f, 0.0f),
    rotation(0.0f, 0.0f, 0.0f),
    scaleValue(1.0f, 1.0f, 1.0f),
    matrix(1.0f),
    isDirty(true)
{
}

Transformation::~Transformation()
{
}

void Transformation::updateMatrix()
{
    matrix = glm::mat4(1.0f);

    matrix = glm::translate(matrix, position);

    float radX = glm::radians(rotation.x);  // pitch
    float radY = glm::radians(rotation.y);  // yaw
    float radZ = glm::radians(rotation.z);  // roll

    matrix = glm::rotate(matrix, radY, glm::vec3(0.0f, 1.0f, 0.0f));  // yaw
    matrix = glm::rotate(matrix, radX, glm::vec3(1.0f, 0.0f, 0.0f));  // pitch
    matrix = glm::rotate(matrix, radZ, glm::vec3(0.0f, 0.0f, 1.0f));  // roll

    matrix = glm::scale(matrix, scaleValue);

    isDirty = false;
}

void Transformation::setPosition(const glm::vec3& pos)
{
    position = pos;
    isDirty = true;
}

void Transformation::translate(const glm::vec3& offset)
{
    position += offset;
    isDirty = true;
}

void Transformation::setRotation(const glm::vec3& rot)
{
    rotation = rot;
    isDirty = true;
}

void Transformation::rotate(const glm::vec3& axis, float angle)
{
    glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    if (axis.x > 0.5f)
        rotation.x += angle;
    else if (axis.y > 0.5f)
        rotation.y += angle;
    else if (axis.z > 0.5f)
        rotation.z += angle;

    isDirty = true;
}

void Transformation::setScale(const glm::vec3& s)
{
    scaleValue = s;
    isDirty = true;
}

void Transformation::applyScale(const glm::vec3& s)
{
    scaleValue *= s;
    isDirty = true;
}

glm::mat4 Transformation::getMatrix()
{
    if (isDirty)
        updateMatrix();

    return matrix;
}

void Transformation::reset()
{
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scaleValue = glm::vec3(1.0f);
    matrix = glm::mat4(1.0f);
    isDirty = false;
}

glm::vec3 Transformation::getForward() const
{
    glm::mat4 m = const_cast<Transformation*>(this)->getMatrix();
    return -glm::normalize(glm::vec3(m[2]));
}

glm::vec3 Transformation::getRight() const
{
    glm::mat4 m = const_cast<Transformation*>(this)->getMatrix();
    return glm::normalize(glm::vec3(m[0]));
}

glm::vec3 Transformation::getUp() const
{
    glm::mat4 m = const_cast<Transformation*>(this)->getMatrix();
    return glm::normalize(glm::vec3(m[1]));
}