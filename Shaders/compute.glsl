#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout (binding = 0) buffer buffer_InPos {
	vec4 InPos[];
}

layout (binding = 1) buffer buffer_OutPos {
	vec4 OutPos[];
}

void main() {
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	vec4 posi = InPos[index];
	posi.x += 0.1;
	OutPos[index] = posi;
}
