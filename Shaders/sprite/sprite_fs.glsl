#version 410 core

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;
uniform vec4 color;
uniform vec4 colorKey;
uniform bool useColorKey;

void main(void) {
	vec4 color = texture(tex, Texcoord) * color;
	
	if(useColorKey) {
		if(color.rgb == vec3(colorKey))
			discard;
	}
	
	outColor = color;
}
