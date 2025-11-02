#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec4 worldPosition;
out vec3 worldNormal;

void main() {
    worldPosition = modelMatrix * vec4(vp, 1.0);
    worldNormal = normalMatrix * vn;
    
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}