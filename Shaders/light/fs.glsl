#version 410 core

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
	sampler2D texture0;
	sampler2D texture1;
	bool isTexture0;
	bool isTexture1;
}; 

struct Light {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	float constant;
	float linear;
	float quadratic;
};

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 uvs;
} fs_in;

out vec4 outColor;
uniform Material material;
uniform Light light; 
uniform vec3 cameraPosition;

void main(void) {
	// ambient
	vec4 ambient = light.ambient;
	if(material.isTexture0) {
		ambient *= texture(material.texture0, fs_in.uvs);
	} else {
		ambient *= material.ambient;
	}

	// diffuse
	vec3 norm = normalize(fs_in.normal);
	vec3 lightDir = normalize(light.position - fs_in.position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = light.diffuse * diff;
	if(material.isTexture0) {
		diffuse *= texture(material.texture0, fs_in.uvs);
	} else {
		diffuse *= material.diffuse;
	}

	// specular
	vec3 viewDir = normalize(cameraPosition - fs_in.position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = light.specular * spec;
	if(material.isTexture1) {
		specular *= texture(material.texture1, fs_in.uvs);
	} else {
		specular *= material.specular;
	}

	// attenuation
	float distance = length(light.position - fs_in.position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec4 result = ambient + diffuse + specular;
	outColor = result;
}
