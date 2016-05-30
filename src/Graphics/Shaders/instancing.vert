#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 instanceMatrix;

layout (std140) uniform Matrices
{
    mat4 projection2;
    mat4 view2;
};

uniform mat4 projection;
uniform mat4 view;

out VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
} vs_out;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(position, 1.0f); 
    vs_out._fragPos = vec3(instanceMatrix * vec4(position, 1.0f));
    vs_out._normal = vec3(transpose(inverse(instanceMatrix)) * vec4(normal, 0.0f));
    //vs_out._normal = normal;
    //vs_out._normal = vec3(0.0f, 1.0f, 0.0f);
    vs_out._texCoords = texCoords;
}