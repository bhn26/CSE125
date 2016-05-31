#include "Grass.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "client/Window.h"
#include "../Scene.h"
#include "../PointLight.h"
#include "../Camera.h"
#include "../../client/Player.h"
#include "../Model.h"
#include "../ModelManager.h"

Grass::Grass(const GLchar* path, GLuint num) : Entity()
{
	//grass = new Model(path);
	grass = ModelManager::GetModel("Grass");
	amount = num;
	shader = ShaderManager::Instance()->GetShader("Instancing");

	// Generate large list of semi-random transformation matrices
	amount = 10000;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed
	GLfloat radius = 150.0f;
	GLfloat offset = 25.0f;
	int index = 0;
	for (GLfloat i = -50.0f; i < 50.0f; i++)
	{
		for (GLfloat j = -50.0f; j < 50.0f; j++) {
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, i));
			model = glm::translate(model, glm::vec3(j, 0.0f, 0.0f));
			GLfloat scale = ((rand() % 20) / 100.0f + 0.05) * 5.0f;
			model = glm::scale(model, glm::vec3(scale));

			GLfloat rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 0.8f, 0.0f));

			modelMatrices[index] = model;
			index++;
		}
	}

	for (GLuint i = 0; i < grass->Meshes().size(); i++)
	{
		GLuint VAO = grass->Meshes()[i].VAO();
        GLuint buffer;
        glBindVertexArray(VAO);
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
        // Set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3); 
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
        glEnableVertexAttribArray(4); 
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5); 
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6); 
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

Grass::~Grass()
{
	delete[] modelMatrices;
}

void Grass::Draw() const
{
	// Set frame time
	//GLfloat currentFrame = glfwGetTime();
	//deltaTime = currentFrame - lastFrame;
	//lastFrame = currentFrame;

	// Draw the loaded model
	shader->Use();
	GLint viewLoc = shader->GetUniform("view");
	glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
	GLint modelLocation = shader->GetUniform("model");
	GLint projectionLocation = shader->GetUniform("projection");

	glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
	glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

	for (GLuint i = 0; i < grass->Meshes().size(); i++) {
		glBindTexture(GL_TEXTURE_2D, grass->Textures()[i].id);
		glBindVertexArray(grass->Meshes()[i].VAO());
		glDrawElementsInstanced(GL_TRIANGLES, grass->Meshes()[i].vertices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}

	//model->Draw(shader.get());
}

void Grass::Update(float deltaTime)
{
}
