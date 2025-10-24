#version 330 core
#define MAX_LIGHTS 10

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

in vec4 worldPosition;
in vec3 worldNormal;

uniform vec3 cameraPosition;
uniform vec3 objectColor;
uniform float shininess;
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

out vec4 out_Color;

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 viewDir = normalize(cameraPosition - worldPosition.xyz);
    
    vec3 ambient = 0.1 * objectColor;
    
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);
    
    for(int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        float distance = length(lights[i].position - worldPosition.xyz);
        
        float attenuation = 1.0 / (lights[i].constant + 
                                   lights[i].linear * distance + 
                                   lights[i].quadratic * distance * distance);

        // Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = attenuation * diff * lights[i].color * lights[i].intensity * objectColor;
        
        // Specular (Phong)
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = attenuation * spec * lights[i].color * lights[i].intensity;
        
        totalDiffuse += diffuse;
        totalSpecular += specular;
    }
    
    vec3 result = ambient + totalDiffuse + totalSpecular;
    out_Color = vec4(result, 1.0);
}