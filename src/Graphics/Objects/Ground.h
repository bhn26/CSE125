#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // glm::mat4
#include <memory>

#include "../Shader.h"

class Ground
{
public:
    glm::mat4 toWorld;
    glm::mat3 normalMatrix;
    glm::vec3 color;
    GLuint VBO, VAO, EBO;
    std::shared_ptr<Shader> shader;

    Ground();
    ~Ground();

    void Draw(glm::mat4 C);
};