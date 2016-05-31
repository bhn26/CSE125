#include "Grass.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "../../client/Window.h"
#include "../Scene.h"
#include "../PointLight.h"
#include "../Camera.h"
#include "../../client/Player.h"
#include "../Model.h"

InstanceObject::InstanceObject(const GLchar* path, GLuint num, GLfloat delta) : Entity()
{
	instance = new Model(path);
	amount = num;
	shader = ShaderManager::Instance()->GetShader("Instancing");

	// Generate large list of semi-random transformation matrices
	amount = 10000;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed
	GLfloat radius = 150.0f;
	GLfloat offset = 25.0f;
	GLfloat size = sqrt(num)/2.0f;

	int index = 0;
	for (GLfloat i = -size; i < size; i++)
	{
		for (GLfloat j = -size; j < size; j++) {
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, i * delta));
			model = glm::translate(model, glm::vec3(j * delta, 0.0f, 0.0f));
			GLfloat scale = ((rand() % 20) / 100.0f + 0.05) * 5.0f;
			model = glm::scale(model, glm::vec3(scale));

			GLfloat rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 0.8f, 0.0f));

			modelMatrices[index] = model;
			index++;
		}
	}

	for (GLuint i = 0; i < instance->Meshes().size(); i++)
	{
		GLuint VAO = instance->Meshes()[i].VAO();
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

InstanceObject::~InstanceObject()
{
	delete[] modelMatrices;
}

void InstanceObject::Draw() const
{
	// Draw the loaded model
	shader->Use();
	GLint viewLoc = shader->GetUniform("view");
	glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
	GLint modelLocation = shader->GetUniform("model");
	GLint projectionLocation = shader->GetUniform("projection");

	shader->Use();
	glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
	glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

	for (GLuint i = 0; i < instance->Meshes().size(); i++) {
		glBindTexture(GL_TEXTURE_2D, instance->Textures()[i].id);
		glBindVertexArray(instance->Meshes()[i].VAO());
		glDrawElementsInstanced(GL_TRIANGLES, instance->Meshes()[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}

	//model->Draw(shader.get());
}

void InstanceObject::Update(float deltaTime)
{
}
