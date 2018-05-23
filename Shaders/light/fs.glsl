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

struct DirLight {
	vec3 direction;
	vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
	vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 uvs;
} fs_in;

#define NUM_POINT_LIGHTS 4

out vec4 outColor;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;

// function prototypes
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void) {
	vec3 norm = normalize(fs_in.normal);
	vec3 viewDir = normalize(viewPos - fs_in.position);

	vec4 result = CalcDirLight(dirLight, norm, viewDir);
	//for(int i = 0; i < NUM_POINT_LIGHTS; i++) {
	//	result += CalcPointLight(pointLights[i], norm, fs_in.position, viewDir);
	//}
	//result += CalcSpotLight(spotLight, norm, fs_in.position, viewDir);
	outColor = result;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec4 ambient = light.ambient;
	vec4 diffuse = light.diffuse * diff;
	vec4 specular = light.specular * spec;
	
	if(material.isTexture0) {
		ambient *= texture(material.texture0, fs_in.uvs);
		diffuse *= texture(material.texture0, fs_in.uvs);
	} else {
		ambient *= material.ambient;
		diffuse *= material.diffuse;
	}
	
	if(material.isTexture1) {
		specular *= texture(material.texture1, fs_in.uvs);
	} else {
		specular *= material.specular;
	}
	
	return (ambient + diffuse + specular);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec4 ambient = light.ambient;
	vec4 diffuse = light.diffuse * diff;
	vec4 specular = light.specular * spec;
	
	if(material.isTexture0) {
		ambient *= texture(material.texture0, fs_in.uvs);
		diffuse *= texture(material.texture0, fs_in.uvs);
	} else {
		ambient *= material.ambient;
		diffuse *= material.diffuse;
	}
	
	if(material.isTexture1) {
		specular *= texture(material.texture1, fs_in.uvs);
	} else {
		specular *= material.specular;
	}
	
	ambient *= attenuation;
	diffuse *= attenuation; 
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	vec4 ambient = light.ambient;
	vec4 diffuse = light.diffuse * diff;
	vec4 specular = light.specular * spec;
	
	if(material.isTexture0) {
		ambient *= texture(material.texture0, fs_in.uvs);
		diffuse *= texture(material.texture0, fs_in.uvs);
	} else {
		ambient *= material.ambient;
		diffuse *= material.diffuse;
	}
	
	if(material.isTexture1) {
		specular *= texture(material.texture1, fs_in.uvs);
	} else {
		specular *= material.specular;
	}
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity; 
	specular *= attenuation * intensity;
	
	return (ambient + diffuse + specular);
}