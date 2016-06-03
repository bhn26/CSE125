#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform int health_x_pos; // x pos between green and red, initialized to 0 by default

uniform sampler2D image;

void main()
{    
	vec4 red_health = vec4(220.0f/255.0f, 44.0f/255.0f, 75.0f/255.0f, 1.0f);
	vec4 img_color = texture(image, TexCoords);

	if((gl_FragCoord.x < health_x_pos) && (img_color == red_health)) {

		color = vec4(0.0f, 166.0f/255.0f, 81.0f/255.0f, 1.0f); // green

	} else {
		color = texture(image, TexCoords);
	}
}	