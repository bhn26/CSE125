#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class Texture;
class Shader;

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer() = default;
    SpriteRenderer(std::shared_ptr<Shader> shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture* texture,
                    glm::vec2 position,
                    glm::vec2 size = glm::vec2(10, 10),
                    GLfloat rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));
    void RenderSelection(int selection_code,
                         Texture* texture,
                         glm::vec2 position,
                         glm::vec2 size = glm::vec2(10, 10),
                         GLfloat rotate = 0.0f);

    std::shared_ptr<Shader> GetShader() { return m_shader; }

private:
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();

    // Render state
    // need to wait for glew init before initializing
    bool m_initialized = false;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Shader> m_selectionShader;
    GLuint m_quadVAO;
};

#endif