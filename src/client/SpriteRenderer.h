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
	SpriteRenderer();
    SpriteRenderer(std::shared_ptr<Shader> shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	void RenderSelection(int selection_code, Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f);

	std::shared_ptr<Shader> GetShader() { return shader; }
private:
    // Render state
	bool initialized;
    std::shared_ptr<Shader> shader; 
    std::shared_ptr<Shader> selectionShader;
    GLuint quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif