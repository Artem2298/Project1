#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;

out vec4 out_Color;

void main(void) {
    vec3 lightToVector = lightPosition - worldPosition.xyz;
    float distance = length(lightToVector);
    vec3 lightDirection = normalize(lightToVector);
    
    vec3 normal = normalize(worldNormal);
    
    float diffuseFactor = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diffuseFactor * lightColor * lightIntensity;
    
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    
    vec3 finalColor = ambient + diffuse;
    
    out_Color = vec4(finalColor, 1.0);
}