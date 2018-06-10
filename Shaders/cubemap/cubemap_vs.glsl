#version 410 core

in vec3 position;
out vec3 uvs;

uniform mat4 view;
uniform mat4 projection;

void main() {
	vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;
	uvs = position;
}