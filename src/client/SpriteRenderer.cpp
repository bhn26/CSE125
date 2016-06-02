#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SpriteRenderer.h"
#include "Window.h"

#include "../Graphics/Texture.h"
#include "../Graphics/Shader.h"

SpriteRenderer::SpriteRenderer() {
	this->shader = new Shader();
	this->selectionShader = new Shader();
	this->initialized = false; // need to wait for glew init before initializing 
}

SpriteRenderer::SpriteRenderer(Shader * shader) // not used
{
    this->shader = shader;
    this->initRenderData();

	this->initialized = true;
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
	delete shader;
	delete selectionShader;
}

void SpriteRenderer::DrawSprite(Texture &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	if (!initialized) {
		shader = new Shader("src/Graphics/Shaders/sprite.vert", "src/Graphics/Shaders/sprite.frag");
		selectionShader = new Shader("src/Graphics/Shaders/selection.vert", "src/Graphics/Shaders/selection.frag");

		initRenderData();
		initialized = true;
	}

    // Prepare transformations
    this->shader->Use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

	GLint model_loc = shader->GetUniform("model");
	glUniformMatrix4fv(model_loc, 1, false, glm::value_ptr(model));

    // Render textured quad
	GLint image_loc = shader->GetUniform("image");
	glUniform1i(image_loc, 0);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Window::width),
		static_cast<GLfloat>(Window::height), 0.0f, -1.0f, 1.0f);
	GLint projection_loc = shader->GetUniform("projection");
	glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));

    //glActiveTexture(GL_TEXTURE0);
    texture.Bind(GL_TEXTURE0);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::RenderSelection(int selection_code, Texture &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate) {
	assert(initialized);
	initRenderData();

	// Prepare transformations
	this->selectionShader->Use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

	GLint model_loc = selectionShader->GetUniform("model");
	glUniformMatrix4fv(model_loc, 1, false, glm::value_ptr(model));

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Window::width),
		static_cast<GLfloat>(Window::height), 0.0f, -1.0f, 1.0f);
	GLint projection_loc = selectionShader->GetUniform("projection");
	glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));

	GLint code_loc = selectionShader->GetUniform("code");
	glUniform1i(code_loc, selection_code);

	texture.Bind(GL_TEXTURE0);

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = { 
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}