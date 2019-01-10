#version 330 core

struct Material {
	//vec3 ambient;
	vec3 diffuse;
	//vec3 specular;
	//sampler2D ambientMap;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	//bool hasAmbient;
	bool hasDiffuseMap;
	//bool hasSpecular;
	bool hasNormalMap;
};

// Uniforms
uniform Material material;
uniform vec3 lightColor;

// Varyings
in vertexData
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
	vec3 lightDir;
	vec3 eyeDir;
} pass;

// Targets
layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 objectColor = texture(material.diffuseMap, pass.texCoords).rgb;

	vec3 L = pass.lightDir;
	vec3 V = pass.eyeDir;

	// ambient
    float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
	vec3 N;
	if (material.hasNormalMap)
		N = normalize(texture(material.normalMap, pass.texCoords).rgb * 2.0 - 1.0);
	//else
		//N = normalize(pass.normal);

    float diff = max(dot(N, L), 0.0);

    vec3 diffuse;
	if (material.hasDiffuseMap)
		diffuse = diff * lightColor;
	//else
	//	diffuse = diff * material.diffuse * lightColor;

    // specular
	float specularStrength = 0.5;
	vec3 halfwayDir = normalize(L + V);
    float spec = pow(max(dot(N, halfwayDir), 0.0), 32.0);

	vec3 specular;
	//if (material.hasSpecular)
	//	specular = texture(material.specularMap, pass.texCoords).rgb * spec;
	//else
		//specular = spec * material.specular * lightColor;
		specular = spec * lightColor;
        
    vec3 result = N;//(ambient + diffuse + specular) * objectColor;
	fragColor = vec4(result, 1.0);
}