#include "Grass.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "../../client/Window.h"
#include "../Scene.h"
#include "../PointLight.h"
#include "../Camera.h"
#include "../../client/Player.h"
#include "../Model.h"

Grass::Grass() : Entity(glm::vec3(0.0f, 0.0f, 0.0f))
{
	shader->Use();

	model = new Model("assets/map/objects/stump.obj");

	GLint viewLoc = shader->GetUniform("view");
	GLint modelLocation = shader->GetUniform("model");
	GLint projectionLocation = shader->GetUniform("projection");

	glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
	glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
	glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

	// Generate large list of semi-random transformation matrices
	amount = 100000;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed
	GLfloat radius = 150.0f;
	GLfloat offset = 25.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;

		// Translate and displace models
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = 0;
		GLfloat z = displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// Add random rotation around a semi-randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// Add to list of matrices
		modelMatrices[i] = model;
	}

	for (GLuint i = 0; i < model->meshes.size(); i++)
	{
		GLuint VAO = model->meshes[i].VAO;
		GLuint buffer;
		glBindVertexArray(VAO);
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		// Set attribute pointers for matrix (4 * vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);

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
	// Draw the loaded model
	shader->Use();
	GLint objectColorLoc = shader->GetUniform("objectColor");
	GLint lightColorLoc = shader->GetUniform("lightColor");
	GLint lightPosLoc = shader->GetUniform("lightPos");
	GLint viewPosLoc = shader->GetUniform("viewPos");

	/*glUniform3fv(objectColorLoc, 1, glm::value_ptr(this->color));
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->color));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->position));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(Scene::Instance()->GetCameraPosition()));*/

	glBindTexture(GL_TEXTURE_2D, model->textures_loaded[0].id);
	for (GLuint i = 0; i < model->meshes.size(); i++) {
		glBindVertexArray(model->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, model->meshes[i].vertices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}

	//model->Draw(shader.get());
}

void Grass::Update()
{
	//Spin(0.3f);
}

void Grass::Spawn(float x, float y, float z)
{
}
