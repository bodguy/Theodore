#version 430 core

out vec4 color;

layout (binding = 1) uniform sampler2D tex_color;

in TES_OUT {
  vec2 tc;
} fs_in;

void main(void) {
  color = texture(tex_color, fs_in.tc);
}
