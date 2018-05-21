#version 410 core

out vec4 outColor;
in vec4 normal_color;

void main(void) {
	outColor = normal_color;
}
