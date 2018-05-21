#version 410 core

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform Light light; 

out vec4 outColor;

in VS_OUT {
	vec3 position;
	vec3 normal;
} fs_in;

uniform vec3 cameraPosition;

void main(void) {
	// ambient
	vec4 ambient = light.ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(fs_in.normal);
	vec3 lightDir = normalize(light.position - fs_in.position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(cameraPosition - fs_in.position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = light.specular * (spec * material.specular);

	vec4 result = ambient + diffuse + specular;
	outColor = result;
}
