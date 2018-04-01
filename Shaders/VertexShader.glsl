#version 410 core

in vec3 position;
in vec4 inColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;

void main() {
    //gl_Position = projection * view * model * vec4(position, 1.0f);
	gl_Position = model * vec4(position, 1.0f);
	vertexColor = inColor;
}