#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexNormal;
out vec3 vertexPosition;

void main()
{
    vec4 worldPos = model * vec4(position, 1.0);
    gl_Position = projection * view * worldPos;
    
    vertexNormal = normal;
    vertexPosition = worldPos.xyz;
}