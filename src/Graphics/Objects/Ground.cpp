#include "Ground.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "client/Window.h"
#include "Graphics/Scene.h"
#include "Graphics/Lights.h"
#include "Graphics/Camera.h"
#include "client/Player.h"

const float Ground::s_size = 400.0f;

Ground::Ground() : Entity(), m_color(glm::vec3(0.545f, 0.271f, 0.075f))
{
    m_normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_toWorld)));

    GLfloat vertices[] =
    {
        -s_size, 0.0f, -s_size,  0.0f,  1.0f, 0.0f,
        s_size,  0.0f, -s_size,  0.0f,  1.0f, 0.0f,
        -s_size, 0.0f,  s_size,  0.0f,  1.0f, 0.0f,
        s_size,  0.0f,  s_size,  0.0f,  1.0f, 0.0f,
    };

    GLuint indices[] =
    {
        0, 1, 2,
        3, 2, 1
    };

    // Create buffers/arrays
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered m_VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind m_VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the m_EBO, keep it bound to this m_VAO
}

Ground::~Ground()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Ground::SetShaderUniforms() const
{
    glUniformMatrix4fv(m_shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(m_shader->GetUniform("model"), 1, false, glm::value_ptr(m_toWorld));
    glUniformMatrix3fv(m_shader->GetUniform("normalMatrix"), 1, false, glm::value_ptr(m_normalMatrix));
    glUniformMatrix4fv(m_shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

    glUniform3fv(m_shader->GetUniform("objectColor"), 1, glm::value_ptr(m_color));
    glUniform3fv(m_shader->GetUniform("lightColor"), 1, glm::value_ptr(Scene::Instance()->GetPointLight()->m_color));
    glUniform3fv(m_shader->GetUniform("lightPos"), 1, glm::value_ptr(Scene::Instance()->GetPointLight()->m_position));
    glUniform3fv(m_shader->GetUniform("viewPos"), 1, glm::value_ptr(Scene::Instance()->GetCameraPosition()));

}

void Ground::Draw() const
{
    // Use the appropriate m_shader (depth or model)
    UseShader();

    // Draw the loaded model
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}