#ifndef _CUBE_H_
#define _CUBE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // glm::mat4
#include <memory>

#include "Entity.h"

class Cube : public Entity
{
public:
    Cube();
    ~Cube();

    glm::vec3 color;

    void Draw() const override;
    void Update(float deltaTime) override;
    void Spin(float);
};

#endif

