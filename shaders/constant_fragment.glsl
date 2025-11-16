#version 330 core

in vec2 uv;

uniform vec3 objectColor;
uniform sampler2D textureUnitID;
uniform int useTexture;

out vec4 out_Color;

void main() {
    if (useTexture == 1) {
        out_Color = texture(textureUnitID, uv);
    } else {
        out_Color = vec4(objectColor, 1.0);
    }
}