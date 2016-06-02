#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 instanceMatrix;

layout (std140) uniform Matrices
{
    mat4 projection2;
    mat4 view2;
    mat4 lightSpaceMatrix2;
};

uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

out VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
    vec4 _fragPosLightSpace;
} vs_out;

void main()
{
    gl_Position = projection2 * view2 * instanceMatrix * vec4(position, 1.0f); 
    vs_out._fragPos = vec3(instanceMatrix * vec4(position, 1.0f));
    vs_out._normal = vec3(transpose(inverse(instanceMatrix)) * vec4(normal, 0.0f));
    vs_out._texCoords = texCoords;
    vs_out._fragPosLightSpace = lightSpaceMatrix2 * vec4(vs_out._fragPos, 1.0f);
}