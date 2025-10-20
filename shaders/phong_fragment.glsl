#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 cameraPosition;
uniform vec3 objectColor;
uniform float shininess;

out vec4 out_Color;

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition.xyz);
    vec3 viewDir = normalize(cameraPosition - worldPosition.xyz);
    
    // Ambient
    vec3 ambient = 0.1 * objectColor;
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightIntensity * objectColor;
    
    // Specular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor * lightIntensity;
    
    vec3 result = ambient + diffuse + specular;
    out_Color = vec4(result, 1.0);
}