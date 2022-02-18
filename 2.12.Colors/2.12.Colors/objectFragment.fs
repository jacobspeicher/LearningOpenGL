#version 330 core

// Phong Shading
 in vec3 Normal;
 in vec3 FragPos;
 in vec3 LightPos;   // import light pos for view-space lighting

 uniform vec3 objectColor;
 uniform vec3 lightColor;
 uniform vec3 lightPos;
 uniform vec3 viewPos;

out vec4 FragColor;

// Gouraud Shading
//in vec3 Color;

void main()
{
    // Phong Shading
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);

    // Gouraud Shading
    //FragColor = vec4(Color, 1.0f);
}