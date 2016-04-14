#ifndef _CUBE_H_
#define _CUBE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // glm::mat4
#include <memory>

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

    std::shared_ptr<Shader> shader;

    void Draw(glm::mat4 C);
    void Update();
    void Spin(float);
};

#endif

