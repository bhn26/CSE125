#include "Ground.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "client/Window.h"
#include "Graphics/Scene.h"
#include "Graphics/Lights.h"
#include "Graphics/Camera.h"
#include "client/Player.h"

const float Ground::SIZE = 400.0f;

Ground::Ground() : Entity(), color(glm::vec3(0.545f, 0.271f, 0.075f))
{
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));

    GLfloat vertices[] =
    {
        -SIZE, 0.0f, -SIZE,  0.0f,  1.0f, 0.0f,
        SIZE,  0.0f, -SIZE,  0.0f,  1.0f, 0.0f,
        -SIZE, 0.0f,  SIZE,  0.0f,  1.0f, 0.0f,
        SIZE,  0.0f,  SIZE,  0.0f,  1.0f, 0.0f,
    };

    GLuint indices[] =
    {
        0, 1, 2,
        3, 2, 1
    };

    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

Ground::~Ground()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Ground::Update(float deltaTime)
{
}

void Ground::SetShaderUniforms() const
{
    glUniformMatrix4fv(shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(shader->GetUniform("normalMatrix"), 1, false, glm::value_ptr(this->normalMatrix));
    glUniformMatrix4fv(shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

    glUniform3fv(shader->GetUniform("objectColor"), 1, glm::value_ptr(this->color));
    glUniform3fv(shader->GetUniform("lightColor"), 1, glm::value_ptr(Scene::Instance()->GetPointLight()->_color));
    glUniform3fv(shader->GetUniform("lightPos"), 1, glm::value_ptr(Scene::Instance()->GetPointLight()->_position));
    glUniform3fv(shader->GetUniform("viewPos"), 1, glm::value_ptr(Scene::Instance()->GetCameraPosition()));

}

void Ground::Draw() const
{
    // Use the appropriate shader (depth or model)
    UseShader();

    // Draw the loaded model
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}