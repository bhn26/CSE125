#ifndef _CUBE_H_
#define _CUBE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/vec3.hpp>

#include "../Shader.h"

class Cube
{
public:
    Cube();
    ~Cube();

    glm::mat4 toWorld;
    glm::mat3 normalMatrix;
    glm::vec3 color;
    GLuint VBO, VAO, EBO;
    float angle;

    Shader shader;

    void draw(glm::mat4 C);
    void update();
    void spin(float);
};

#endif

