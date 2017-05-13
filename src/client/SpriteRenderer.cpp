#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SpriteRenderer.h"
#include "Window.h"

#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderManager.h"

// not used
SpriteRenderer::SpriteRenderer(std::shared_ptr<Shader> shader) : m_shader(shader), m_initialized(true)
{
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void SpriteRenderer::DrawSprite(Texture& texture,
                                glm::vec2 position,
                                glm::vec2 size,
                                GLfloat rotate,
                                glm::vec3 color)
{
    if (!m_initialized)
    {
        // shader = new Shader("src/Graphics/Shaders/sprite.vert",
        // "src/Graphics/Shaders/sprite.frag");  selectionShader = new
        // Shader("src/Graphics/Shaders/selection.vert", "src/Graphics/Shaders/selection.frag");
        m_shader = ShaderManager::GetShader("Sprite");
        m_selectionShader = ShaderManager::GetShader("Selection");

        initRenderData();
        m_initialized = true;
    }

    // Prepare transformations
    m_shader->Use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f)); // First translate (transformations
                                                              // are: scale happens first, then
                                                              // rotation and then finall
                                                              // translation happens; reversed
                                                              // order)

    model = glm::translate(
        model,
        glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model =
        glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

    GLint model_loc = m_shader->GetUniform("model");
    glUniformMatrix4fv(model_loc, 1, false, glm::value_ptr(model));

    // Render textured quad
    GLint image_loc = m_shader->GetUniform("image");
    glUniform1i(image_loc, 0);

    glm::mat4 projection = glm::ortho(0.0f,
                                      static_cast<GLfloat>(Window::s_width),
                                      static_cast<GLfloat>(Window::s_height),
                                      0.0f,
                                      -1.0f,
                                      1.0f);
    GLint projection_loc = m_shader->GetUniform("projection");
    glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));

    // glActiveTexture(GL_TEXTURE0);
    texture.Bind(GL_TEXTURE0);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::RenderSelection(int selection_code,
                                     Texture& texture,
                                     glm::vec2 position,
                                     glm::vec2 size,
                                     GLfloat rotate)
{
    assert(m_initialized);
    initRenderData();

    // Prepare transformations
    m_selectionShader->Use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f)); // First translate (transformations
                                                              // are: scale happens first, then
                                                              // rotation and then finall
                                                              // translation happens; reversed
                                                              // order)

    model = glm::translate(
        model,
        glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model =
        glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

    GLint model_loc = m_selectionShader->GetUniform("model");
    glUniformMatrix4fv(model_loc, 1, false, glm::value_ptr(model));

    glm::mat4 projection = glm::ortho(0.0f,
                                      static_cast<GLfloat>(Window::s_width),
                                      static_cast<GLfloat>(Window::s_height),
                                      0.0f,
                                      -1.0f,
                                      1.0f);
    GLint projection_loc = m_selectionShader->GetUniform("projection");
    glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));

    GLint code_loc = m_selectionShader->GetUniform("code");
    glUniform1i(code_loc, selection_code);

    texture.Bind(GL_TEXTURE0);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {// Pos      // Tex
                          0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                          0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}