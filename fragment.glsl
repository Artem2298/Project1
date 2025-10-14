#version 330 core

in vec3 vertexNormal;
in vec3 vertexPosition;

out vec4 fragColor;

void main()
{
    vec3 normalizedNormal = normalize(vertexNormal);
    vec3 normalAsColor = normalizedNormal * 0.5 + 0.5;
    
    fragColor = vec4(normalAsColor, 1.0);
}