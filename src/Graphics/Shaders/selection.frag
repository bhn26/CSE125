#version 330

uniform int code;

out vec4 outputF;

void main()
{
	outputF = vec4(code/255.0);
	//outputF = vec4(1,0,0,1);
} 
