#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// lightPos uniform for view-space lighting
// uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;
// out vec3 LightPos;

// Gouraud Shading
//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
//uniform vec3 viewPos;

//out vec3 Color;

void main()
{
	gl_Position = projection * view * model * vec4(vPos, 1.0f);
	FragPos = vec3(model * vec4(vPos, 1.0f));
	Normal = mat3(transpose(inverse(model))) * vNormal;
	// LightPos = vec3(view * vec4(lightPos, 1.0f));

	// Gouraurd Shading
	//float ambientStrength = 0.1f;
	//vec3 ambient = ambientStrength * lightColor;

	//vec3 norm = normalize(Normal);
	//vec3 lightDirection = normalize(lightPos - FragPos);
	//float diff = max(dot(norm, lightDirection), 0.0f);
	//vec3 diffuse = diff * lightColor;

	//float specularStrength = 1.0f;
	//vec3 viewDirection = normalize(viewPos - FragPos);
	//vec3 reflectDirection = reflect(-lightDirection, norm);
	//float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 4);
	//vec3 specular = specularStrength * spec * lightColor;

	//Color = (ambient + diffuse + specular) * objectColor;
}