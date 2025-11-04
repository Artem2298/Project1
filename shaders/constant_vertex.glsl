#version 330 core
in vec3 vp;
in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;

void main() {
    worldPosition = modelMatrix * vec4(vp, 1.0);
    worldNormal = mat3(modelMatrix) * vn;
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}