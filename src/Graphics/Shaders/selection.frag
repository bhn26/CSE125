#version 330 core

uniform int code;

out vec4 color;

void main()
{
	color = vec4(code/255.0, 0.0f, 0.0f, 1.0f);
} 
