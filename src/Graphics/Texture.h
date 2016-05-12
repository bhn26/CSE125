#pragma once
#include <string>

#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <SOIL.h>

class Texture2D {
private:
	std::string texture_file;

public:
	Texture2D(std::string file) { texture_file = file; };
	~Texture2D() {};

	void Bind() {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// Load and generate the texture
		int width, height;
		unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	};
};