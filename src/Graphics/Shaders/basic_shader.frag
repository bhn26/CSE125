#version 330 core

in VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
} fs_in;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Calculations
    vec3 norm = normalize(fs_in._normal);
    vec3 lightDir = normalize(lightPos - fs_in._fragPos); 
    vec3 viewDir = normalize(viewPos - fs_in._fragPos);
    vec3 reflectDir = reflect(-lightPos, norm);

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
    vec3 specular = specularStrength * spec * lightColor;
    //vec3 specular = vec3(0.0f);

    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}