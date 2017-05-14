#pragma once

// Std. Includes
#include <iostream>
#include <map>
#include <string>
#include <memory>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

class Shader;

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character
{
    GLuint m_textureId;   // ID handle of the glyph texture
    glm::ivec2 m_size;    // Size of glyph
    glm::ivec2 m_bearing; // Offset from baseline to left/top of glyph
    GLuint m_advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer
{
    static std::shared_ptr<Shader> s_shader; // shader for font
    static std::map<GLchar, Character> s_characters;
    static GLuint s_VAO, s_VBO;

public:
    static void Initialize();
    static void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};
