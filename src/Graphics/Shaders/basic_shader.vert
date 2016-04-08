#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model2world;
uniform mat3 normalMatrix;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    gl_Position = vec4(position, 1.0f);
    gl_Position = projection * model2world * gl_Position;
    //gl_Position = model2world * gl_Position;
    FragPos = vec3(model2world * vec4(position, 1.0f));
    Normal = normalMatrix * normal;
}
