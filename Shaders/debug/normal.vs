#version 410 core

in vec3 position;
in vec3 normal;
	
out VS_OUT {
	vec3 position;
	vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void) {
	gl_Position = projection * view * model * vec4(position, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_out.position = vec3(model * vec4(position, 1.0));
	vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * normal, 0.0)));
}
