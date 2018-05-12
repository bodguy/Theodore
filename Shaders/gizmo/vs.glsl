#version 410 core

in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void) {
	gl_Position =  projection * view * model * vec4(position, 1.0);
}
