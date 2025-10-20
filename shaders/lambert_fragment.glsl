#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 objectColor;

out vec4 out_Color;

void main() {
    // Ambient
    vec3 ambient = 0.1 * objectColor;
    
    // Diffuse
    vec3 lightDir = normalize(lightPosition - worldPosition.xyz);
    vec3 normal = normalize(worldNormal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity * objectColor;
    
    vec3 result = ambient + diffuse;
    out_Color = vec4(result, 1.0);
}