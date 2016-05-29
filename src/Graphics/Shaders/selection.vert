#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

// Uses different projection matrix
//layout (std140) uniform Matrices
//{
//    mat4 projection;
//    mat4 view;      // Even though not used
//};

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
