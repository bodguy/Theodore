#version 430 core

layout (location = 0) in vec4 vertices;
out VS_OUT {
  vec2 tc;
} vs_out;

void main(void) {
  int x = gl_InstanceID & 63;
  int y = gl_InstanceID >> 6;
  vec2 offs = vec2(x, y);

  vs_out.tc = (vertices.xz + offs + vec2(0.5)) / 64.0;
  gl_Position = vertices + vec4(float(x - 32), 0.0, float(y- 32), 0.0);
}
