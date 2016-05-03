#version 330 core

out vec4 color;
in vec3 FragPos;
in vec3 Normal;
in vec4 Eye;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

// Object material
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

// Light
uniform vec3 dl_dir;
uniform vec3 dl_ambient;
uniform vec3 dl_diffuse;
uniform vec3 dl_specular;

void main()
{
    // Calculations
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 

	vec4 kala = (ambient * dl_ambient);
	vec4 diff = (diffuse * dl_diffuse) * max(dot(norm, lightDir), 0.0);
	vec4 rVec = reflect(-lightDir, norm);
	vec4 spec = (specular * dl_specular) * pow(max(dot(rVec, Eye), 0.0), shininess);

	color = kala + diff + spec;

    /*float ambientStrength = 0.1f;
    float specularStrength = 0.5f;

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Calculations
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0f);*/
}