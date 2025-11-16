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
in vec2 uv;

uniform vec3 cameraPosition;
uniform vec3 objectColor;
uniform float shininess;

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform SpotLight spotlight;

uniform sampler2D textureUnitID;
uniform int useTexture;

out vec4 out_Color;

void main() {
    vec3 normal = normalize(worldNormal);
    vec3 viewDir = normalize(cameraPosition - worldPosition.xyz);
    
    vec3 baseColor;
    if (useTexture == 1) {
        vec4 texColor = texture(textureUnitID, uv);
        baseColor = texColor.rgb;
    } else {
        baseColor = objectColor;
    }
    
    vec3 ambient = 0.1 * baseColor;
    
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);
    
    for(int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        float distance = length(lights[i].position - worldPosition.xyz);
        
        float attenuation = 1.0 / (lights[i].constant + 
                                   lights[i].linear * distance + 
                                   lights[i].quadratic * distance * distance);
        
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = attenuation * diff * lights[i].color * lights[i].intensity * baseColor;
        
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = attenuation * spec * lights[i].color * lights[i].intensity;
        
        totalDiffuse += diffuse;
        totalSpecular += specular;
    }
    
    if(spotlight.enabled == 1) {
        vec3 lightDir = normalize(spotlight.position - worldPosition.xyz);
        float distance = length(spotlight.position - worldPosition.xyz);
        
        float theta = dot(lightDir, normalize(-spotlight.direction));
        
        float epsilon = spotlight.cutOff - spotlight.outerCutOff;
        float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);
        
        if(theta > spotlight.outerCutOff) {
            float attenuation = 1.0 / (spotlight.constant + 
                                       spotlight.linear * distance + 
                                       spotlight.quadratic * distance * distance);
            
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = attenuation * intensity * diff * spotlight.color * spotlight.intensity * baseColor;
            
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = attenuation * intensity * spec * spotlight.color * spotlight.intensity;
            
            totalDiffuse += diffuse;
            totalSpecular += specular;
        }
    }
    
    vec3 result = ambient + totalDiffuse + totalSpecular;
    out_Color = vec4(result, 1.0);
}