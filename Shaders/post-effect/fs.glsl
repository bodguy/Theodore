#version 430 core

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D screenTexture;

void main() {
    outColor = texture(screenTexture, TexCoords);
}