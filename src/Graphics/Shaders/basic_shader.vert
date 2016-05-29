#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (std140) uniform Matrices
{
    mat4 projection2;
    mat4 view2;
};

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    gl_Position = vec4(position, 1.0f);
    FragPos = vec3(model * gl_Position);
    gl_Position = projection * view * model * gl_Position;
    Normal = normalMatrix * normal;
}
