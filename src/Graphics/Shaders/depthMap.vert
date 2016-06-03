#version 330 core
layout (location = 0) in vec3 position;
layout (location = 3) in mat4 instanceMatrix;

layout (std140) uniform Matrices
{
    mat4 projection2;
    mat4 view2;
    mat4 lightSpaceMatrix2;
};

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform bool instancing;

void main()
{
    if (instancing)
        gl_Position = lightSpaceMatrix * instanceMatrix * vec4(position, 1.0f);
    else
        gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
        //gl_Position = model * vec4(position, 1.0f);
}  