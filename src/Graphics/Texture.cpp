#include "Texture.h"

#include <iostream>
#include <SOIL.h>

Texture::Texture(GLenum textureTarget, const std::string& filename) : m_textureTarget(textureTarget), m_fileName(filename)
{
	glGenTextures(1, &m_textureID);
	Load();
}

Texture::~Texture()
{
	if (m_textureID)
		glDeleteTextures(1, &m_textureID);
}

Texture::Texture(Texture&& rhs) : m_textureID(rhs.m_textureID), m_fileName(std::move(rhs.m_fileName))
{
	rhs.m_textureID = 0;
}

Texture& Texture::operator=(Texture&& rhs)
{
	m_textureID = rhs.m_textureID;
	rhs.m_textureID = 0;
	m_fileName = std::move(rhs.m_fileName);
	return *this;
}

bool Texture::Load()
{
    int width, height;
    unsigned char* image = SOIL_load_image(m_fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	if (!image)
	{
		fprintf(stderr, "Cannot load image: %s\n", m_fileName.c_str());
		return false;
	}
    // Assign texture to ID
    glBindTexture(m_textureTarget, m_textureID);
    glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Reset
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

	this->width = width;
	this->height = height;

    return true;
}

void Texture::Bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(m_textureTarget, m_textureID);
}
