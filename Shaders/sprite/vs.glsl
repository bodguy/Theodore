#version 410 core

in vec3 position;
in vec2 texcoord;

out vec2 Texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool flipX;
uniform bool flipY;

void main(void) {
	vec2 resultTexcoord = texcoord;
	if(flipX) {
		resultTexcoord = vec2(1.0 - resultTexcoord.x, resultTexcoord.y);
	}
	if(flipY) {
		resultTexcoord = vec2(resultTexcoord.x, 1.0 - resultTexcoord.y);
	}
	Texcoord = resultTexcoord;
	gl_Position =  projection * view * model * vec4(position, 1.0);
}
