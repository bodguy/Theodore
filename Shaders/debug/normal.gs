#version 410 core

layout(triangles) in;
layout(line_strip, max_vertices=6) out;

const float magnitude = 0.2;
const float max_render_distance = 100.0;

in VS_OUT {
	vec3 position;
	vec3 normal;
} gs_in[];

out vec4 normal_color;

void GenerateLine(int index) {
	vec4 pos = gl_in[index].gl_Position;
	if(length(pos) < max_render_distance) {
		normal_color = abs(vec4(gs_in[index].normal, 1.0));
		gl_Position = pos;
		EmitVertex();
		gl_Position = pos + vec4(gs_in[index].normal, 0.0) * magnitude;
		EmitVertex();
		EndPrimitive();
	}

}

void main() {		
	// gl_in.length() == 3 for triangles
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}