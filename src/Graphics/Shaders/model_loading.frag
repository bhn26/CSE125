#version 330 core

struct Material
{
    vec3 _diffuse;
    vec3 _specular;
    vec3 _ambient;
    float _shininess;
};

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform bool useTexture;
uniform bool useMaterial;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    color = vec4(1.0f);
    if (useTexture)
    {
        color = vec4(texture(texture_diffuse1, TexCoords));
    }
    if (useMaterial)
    {
        color = color * vec4(material._diffuse, 1.0f);
    }
}