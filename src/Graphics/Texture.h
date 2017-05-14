#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../client/Window.h"

#include <SOIL/SOIL.h>

struct Material
{
    glm::vec3 m_diffuse = glm::vec3(1.0f);
    glm::vec3 m_specular = glm::vec3(1.0f);
    glm::vec3 m_ambient = glm::vec3(1.0f);
    float m_shininess = 0.0f;
    bool Valid() const
    {
        return glm::length(m_diffuse) > 0.0f;
    } // || glm::length(m_specular) || glm::length(m_ambient); }
};

class Texture
{
public:
    Texture(GLenum textureTarget = GL_TEXTURE_2D, const std::string& filename = "");
    ~Texture();
    // Texture(GLenum textureTarget = GLm_TEXTURE0, const std::string& filename = "");

    Texture(const Texture& rhs) = delete;
    Texture& operator=(Texture& rhs) = delete;
    Texture(Texture&& rhs);
    Texture& operator=(Texture&& rhs);

    bool Load();
    // Texture unit like GLm_TEXTURE0
    void Bind(GLenum textureUnit) const;

    int Width() { return width; };
    int Height() { return height; };

    static int GetWindowCenter(int width) { return Window::s_width / 2 - width / 2; };

private:
    std::string m_fileName;
    GLenum m_textureTarget; // What type of texture (probably GLm_TEXTUREm_2D)
    GLuint m_textureID;

    int width = 0;
    int height = 0;
};
