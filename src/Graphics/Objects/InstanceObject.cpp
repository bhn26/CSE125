#include "InstanceObject.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "client/Window.h"
#include "client/Player.h"
#include "../Scene.h"
#include "../Lights.h"
#include "../Camera.h"
#include "../Model.h"

InstanceObject::InstanceObject(std::shared_ptr<Model> instanceModel, GLuint num, GLfloat delta) : Entity(), instance(instanceModel)
{
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
    delete[] modelMatrices;
}

InstanceObject::~InstanceObject()
{
}

void InstanceObject::Draw() const
{
	// Draw the loaded model
	//shader->Use();
    UseShader();

	for (GLuint i = 0; i < instance->Meshes().size(); i++) {
		glBindTexture(GL_TEXTURE_2D, instance->Textures()[i].id);
		glBindVertexArray(instance->Meshes()[i].VAO());
		glDrawElementsInstanced(GL_TRIANGLES, instance->Meshes()[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}
    if (Scene::Instance()->IsRenderingDepth())
    {
        std::shared_ptr<Shader>& shader = Scene::Instance()->GetDepthShader();
        glUniform1i(shader->GetUniform("instancing"), GL_FALSE);
    }
}

void InstanceObject::UseShader() const
{
    // For rendering Depth, only need model
    if (Scene::Instance()->IsRenderingDepth())
    {
        std::shared_ptr<Shader>& shader = Scene::Instance()->GetDepthShader();
        //shader->Use();
        glUniformMatrix4fv(shader->GetUniform("model"), 1, GL_FALSE, glm::value_ptr(toWorld));
        glUniform1i(shader->GetUniform("instancing"), GL_TRUE);
    }
    else
    {
        shader->Use();
        this->SetShaderUniforms();
    }
}

void InstanceObject::SetShaderUniforms() const
{
    glUniformMatrix4fv(shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix4fv(shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

    LoadDirectionalLight(Scene::Instance()->GetDirectionalLight());
}
