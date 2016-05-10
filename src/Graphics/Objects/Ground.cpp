#include "Ground.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "../../client/Window.h"
#include "../Scene.h"
#include "../PointLight.h"
#include "../Camera.h"
#include "../../client/Player.h"


Ground::Ground() : Entity(), color(glm::vec3(0.545f, 0.271f, 0.075f))
{
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));

    GLfloat vertices[] =
    {
        -50.0f, 0.0f, -50.0f,  0.0f,  1.0f, 0.0f,
        50.0f,  0.0f, -50.0f,  0.0f,  1.0f, 0.0f,
        -50.0f, 0.0f,  50.0f,  0.0f,  1.0f, 0.0f,
        50.0f,  0.0f,  50.0f,  0.0f,  1.0f, 0.0f,
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

void Ground::Update()
{
}

void Ground::Spawn(float x, float y, float z)
{
}

void Ground::Draw() const
{
    shader->Use();

    GLint viewLoc = shader->GetUniform("view");
    GLint modelLocation = shader->GetUniform("model");
    GLint normalMatrixLoc = shader->GetUniform("normalMatrix");
    GLint projectionLocation = shader->GetUniform("projection");
    GLint objectColorLoc = shader->GetUniform("objectColor");
    GLint lightColorLoc = shader->GetUniform("lightColor");
    GLint lightPosLoc = shader->GetUniform("lightPos");
    GLint viewPosLoc = shader->GetUniform("viewPos");

    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(normalMatrixLoc, 1, false, glm::value_ptr(this->normalMatrix));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

    glUniform3fv(objectColorLoc, 1, glm::value_ptr(this->color));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->color));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->position));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(Scene::Instance()->GetCameraPosition()));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}