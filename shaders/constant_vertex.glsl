#version 330 core

in vec3 vp;
in vec2 vt;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 uv;

void main() {
    uv = vt;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
}