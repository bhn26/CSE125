#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Material
{
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    glm::vec3 _ambient;
    float _shininess;
    Material() : _diffuse(), _specular(), _ambient(), _shininess(0.0f) {}
};

class Texture
{
public:
    Texture(GLenum textureTarget = GL_TEXTURE_2D, const std::string& filename = "");
	~Texture();
    //Texture(GLenum textureTarget = GL_TEXTURE0, const std::string& filename = "");

	Texture(const Texture& rhs) = delete;
	Texture& operator=(Texture& rhs) = delete;
	Texture(Texture&& rhs);
	Texture& operator=(Texture&& rhs);

    bool Load();
    void Bind(GLenum textureUnit) const;    // Texture unit like GL_TEXTURE0

	int Width() { return width; };
	int Height() { return height; };

private:
    std::string m_fileName;
    GLenum m_textureTarget;     // What type of texture (probably GL_TEXTURE_2D)
    GLuint m_textureID;

	int width;
	int height;
};
