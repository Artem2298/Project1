#version 330 core

in vec3 vp;  // vertex position
in vec3 vn;  // vertex normal
in vec2 vt;  // vertex texcoord

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec4 worldPosition;
out vec3 worldNormal;
out vec2 uv;

void main() {
    worldPosition = modelMatrix * vec4(vp, 1.0);
    worldNormal = normalize(normalMatrix * vn);
    uv = vt;
    
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}