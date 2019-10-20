#version 410 core

in vec2 position;
in vec2 texcoord;
out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position, 0.0, 1.0);
    TexCoords = texcoord;
}