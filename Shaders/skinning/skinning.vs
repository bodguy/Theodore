#version 410 core

const int MAX_BONES = 100;

in vec3 position;
in vec3 normal;
in ivec4 boneIds;
in vec4 weights;

out vec3 fNormal;
out vec3 fPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 Bone[MAX_BONES]; // Array of bones that you compute (animate) on the CPU and you upload to the shader

void main(void) {
    mat4 boneTransform = mat4(0);
	for(uint i = 0; i < 4; i++) {
		boneTransform += Bone[boneIds[i]] * weights[i];
	}

	mat4 modelBone = model * boneTransform;
	vec4 worldpos = modelBone * vec4(position.xyz, 1.0);
	
	gl_Position = projection * view * worldpos;
	fPos = vec3(worldpos);
	fNormal = mat3(transpose(inverse(view * modelBone))) * normal;
}