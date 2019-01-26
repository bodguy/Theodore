#version 410 core

#define NUM_VERTICES 40

layout(points) in;
layout(line_strip, max_vertices=NUM_VERTICES) out;

uniform float radius;
uniform mat4 perspective;

#define M_PI 3.1415926535897932384626433832795

void main() {
    vec4 pos = gl_in[0].gl_Position;
	float precomputed = 2.0 * M_PI / (NUM_VERTICES-1);
	
    for(int i = 0; i < NUM_VERTICES; i++) {
        vec4 newVertex = perspective * vec4(radius * cos(i * precomputed), radius * sin(i * precomputed), 0.0, 1.0);

        gl_Position = vec4(pos.x + newVertex.x, pos.y + newVertex.y, pos.z, pos.w);
        EmitVertex();
    }
    EndPrimitive();
}
