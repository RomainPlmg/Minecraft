#version 330 core

out vec4 FragColor;

in vec3 outColor;
in vec2 outUV;

uniform sampler2D outTexture;

void main() {
    FragColor = texture(outTexture, outUV);
}