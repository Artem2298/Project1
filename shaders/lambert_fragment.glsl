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

uniform vec3 objectColor;
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

out vec4 out_Color;

void main() {

    if (numLights > 0) {
        float distance = length(lights[0].position - worldPosition.xyz);
        
        if (distance < 1.0) {
            out_Color = vec4(1.0, 0.0, 0.0, 1.0);
        } else if (distance < 2.0) {
            out_Color = vec4(1.0, 0.5, 0.0, 1.0);
        } else if (distance < 3.0) {
            out_Color = vec4(1.0, 1.0, 0.0, 1.0);
        } else if (distance < 5.0) {
            out_Color = vec4(0.0, 1.0, 0.0, 1.0);
        } else if (distance < 10.0) {
            out_Color = vec4(0.0, 0.5, 1.0, 1.0);
        } else {
            out_Color = vec4(0.0, 0.0, 1.0, 1.0);
        }
        return;
    }

    vec3 normal = normalize(worldNormal);
    
    vec3 ambient = 0.1 * objectColor;
    
    vec3 totalDiffuse = vec3(0.0);
    
    

    for(int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        float distance = length(lights[i].position - worldPosition.xyz);
        
        float attenuation = 1.0 / (lights[i].constant + 
                                   lights[i].linear * distance + 
                                   lights[i].quadratic * distance * distance);
        
        // Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = attenuation * diff * lights[i].color * lights[i].intensity * objectColor;
        
        totalDiffuse += diffuse;
    }
    
    vec3 result = ambient + totalDiffuse;
    out_Color = vec4(result, 1.0);
}