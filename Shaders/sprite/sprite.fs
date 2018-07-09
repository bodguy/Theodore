#version 410 core

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;
uniform vec4 color;

void main(void) {
	outColor = texture(tex, Texcoord) * color;
}
