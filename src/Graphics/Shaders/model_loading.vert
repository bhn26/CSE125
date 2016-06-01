#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Matrices
{
    mat4 projection2;
    mat4 view2;
};

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
    vec4 _fragPosLightSpace;
} vs_out;

void main()
{
    gl_Position = vec4(position, 1.0f);
    vs_out._fragPos = vec3(model * gl_Position);
    vs_out._normal = normalize(normalMatrix * normal);
    vs_out._texCoords = texCoords;
    vs_out._fragPosLightSpace = lightSpaceMatrix * vec4(vs_out._fragPos, 1.0f);
    gl_Position = projection * view * model * gl_Position;
    //gl_Position = projection2 * view2 * model * gl_Position;
}