#ifndef _CUBE_H_
#define _CUBE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp>

#include "../shader.hpp"

class Cube
{
public:
    Cube();
    ~Cube();

    glm::mat4 toWorld;
    glm::vec3 color;
    GLuint VBO, VAO, EBO;
    float angle;

    Shader shader;

    void draw();
    void update();
    void spin(float);
};

#endif

