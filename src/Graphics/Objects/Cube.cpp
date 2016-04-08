#include "Cube.h"
#include "../Window.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

extern glm::vec3 lightPos;
extern glm::vec3 lightColor;

GLfloat vertices[] = 
{
    // front
    -0.5, -0.5,  0.5, 0.0f, 0.0f, 1.0f,
    0.5, -0.5,  0.5, 0.0f, 0.0f, 1.0f,
    0.5,  0.5,  0.5, 0.0f, 0.0f, 1.0f,
    -0.5,  0.5,  0.5, 0.0f, 0.0f, 1.0f,
    // back
    -0.5, -0.5, -0.5, 0.0f, 0.0f, -1.0f,
    0.5, -0.5, -0.5, 0.0f, 0.0f, -1.0f,
    0.5,  0.5, -0.5, 0.0f, 0.0f, -1.0f,
    -0.5,  0.5, -0.5, 0.0f, 0.0f, -1.0f
};

GLuint indices[] =  // Note that we start from 0!
{
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 0, 3,
    3, 7, 4,
    // left
    4, 5, 1,
    1, 0, 4,
    // right
    3, 2, 6,
    6, 7, 3
};

Cube::Cube()
{
    this->toWorld = glm::mat4(1.0f);
    this->color = glm::vec3(1.0f, 0.2f, 0.1f);

    this->angle = 0.0f;

    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

Cube::~Cube()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::draw(glm::mat4 C)
{
    shader.Use();

    glm::mat4 projection = glm::perspective((GLfloat)45.0f, (GLfloat)Window::width / (GLfloat)Window::height, 0.1f, 100.0f);

    GLint viewLoc = shader.GetUniform("view");
    GLint modelLocation = shader.GetUniform("model2world");
    GLint normalMatrixLoc = shader.GetUniform("normalMatrix");
    GLint projectionLocation = shader.GetUniform("projection");
    GLint objectColorLoc = shader.GetUniform("objectColor");
    GLint lightColorLoc = shader.GetUniform("lightColor");
    GLint lightPosLoc = shader.GetUniform("lightPos");

    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(C));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(normalMatrixLoc, 1, false, glm::value_ptr(this->normalMatrix));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));

    glUniform3fv(objectColorLoc, 1, glm::value_ptr(this->color));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::update()
{
    spin(0.3f);
}

void Cube::spin(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f)
        this->angle = 0.0f;

    // This creates the matrix to rotate the cube
    this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    this->normalMatrix = glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

