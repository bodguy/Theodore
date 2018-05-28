#version 410 core

in vec3 position;
in vec2 uvs;

out vec2 TexCoords;

void main() {
	TexCoords = uvs;
	gl_Position = vec4(position, 1.0);
}