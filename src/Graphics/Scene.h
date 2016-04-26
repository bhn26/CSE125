#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>

#include "Objects/Entity.h"

class Camera;
class Player;

struct PointLight;

namespace Scene
{
    extern std::unique_ptr<Camera> camera;
    extern std::unique_ptr<PointLight> pLight;
    extern Player* player;

    extern std::vector<std::unique_ptr<Entity>> entities;

    void Setup();
    void Dealloc();

    void Update();
    void Draw();

    // Interface to camera
    glm::mat4 GetViewMatrix();
    glm::vec3 GetCameraPosition();
    glm::mat4 GetPerspectiveMatrix();
}