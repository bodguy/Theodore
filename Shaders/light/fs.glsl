#version 430 core

in vec3 fNormal;
in vec3 fPos;
out vec4 outColor;

const vec3 lightColor = vec3(1.0f, 0.84f, 0.0f);
uniform vec4 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void) {
	// ambient color
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse color
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular color
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor.xyz;
	outColor = vec4(result, 1.0);
}
