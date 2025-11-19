#version 330 core

in vec3 vp;
in vec3 vn;
in vec2 vt;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec4 worldPosition;
out vec3 worldNormal;
out vec2 TexCoord;

void main() {
    const float w = 200.0;
    vec4 scaleVector = vec4(vp, 1.0) * w;
    worldPosition = (modelMatrix * scaleVector) / w;
    worldNormal = normalize(normalMatrix * vn);
    TexCoord = vt;
    
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}