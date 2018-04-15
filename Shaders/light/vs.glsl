#version 430 core

in vec3 position;
in vec3 normal;
out vec3 fNormal;
out vec3 fPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void) {
	gl_Position =  projection * view * model * vec4(position, 1.0);
	fPos = vec3(model * vec4(position, 1.0));
	fNormal = mat3(transpose(inverse(view * model))) * normal;
}
