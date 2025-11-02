#version 330 core

#define MAX_LIGHTS 20

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    int enabled;
};

in vec4 worldPosition;
in vec3 worldNormal;

uniform vec3 objectColor;

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform SpotLight spotlight;

out vec4 out_Color;

void main() {
    vec3 normal = normalize(worldNormal);
    
    vec3 ambient = 0.1 * objectColor;
    vec3 totalDiffuse = vec3(0.0);
    
    for(int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        float distance = length(lights[i].position - worldPosition.xyz);
        
        float attenuation = 1.0 / (lights[i].constant + 
                                   lights[i].linear * distance + 
                                   lights[i].quadratic * distance * distance);
        
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = attenuation * diff * lights[i].color * lights[i].intensity * objectColor;
        
        totalDiffuse += diffuse;
    }
    
    if(spotlight.enabled == 1) {
        vec3 lightDir = normalize(spotlight.position - worldPosition.xyz);
        float distance = length(spotlight.position - worldPosition.xyz);
        
        float theta = dot(lightDir, normalize(-spotlight.direction));
        
        if(theta > spotlight.outerCutOff) {
            float epsilon = spotlight.cutOff - spotlight.outerCutOff;
            float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);
            
            float attenuation = 1.0 / (spotlight.constant + 
                                       spotlight.linear * distance + 
                                       spotlight.quadratic * distance * distance);
            
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = attenuation * intensity * diff * spotlight.color * spotlight.intensity * objectColor;
            
            totalDiffuse += diffuse;
        }
    }
    
    vec3 result = ambient + totalDiffuse;
    out_Color = vec4(result, 1.0);
}