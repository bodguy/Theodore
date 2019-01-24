#version 410 core

in vec3 uvs;
out vec4 outColor;

uniform samplerCube cubemap;

void main() {
	outColor = texture(cubemap, uvs);
}