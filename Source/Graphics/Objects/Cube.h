#ifndef _CUBE_H_
#define _CUBE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4

class Cube
{
public:
    Cube();
    ~Cube();

    glm::mat4 toWorld;
    GLuint VBO, VAO, EBO;
    float angle;

    void draw(GLuint);
    void update();
    void spin(float);
};

#endif

