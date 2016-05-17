#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
    color = texture(texture_diffuse1, TexCoords);
}

/*
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture_diffuse1;

struct Material
{
    vec3 _diffuse;
    vec3 _ambient;
    vec3 _specular;
    float _shininess
};

uniform Material material;
uniform bool useTexture;

void main()
{
    if (useTexture)
        color = texture(texture_diffuse1, TexCoords);
    else
        color = material._diffuse;        // TODO LIGHTING EQUATIONS
}
*/