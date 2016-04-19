#pragma once
#include <memory>
#include <glm/glm.hpp>

class Camera;
class Cube;
class Chicken;
class Ground;
class Player;
class CubeMap;

struct PointLight;

namespace Scene
{
    extern std::unique_ptr<Camera> camera;
    extern std::unique_ptr<Cube> cube;
    extern std::unique_ptr<Chicken> chicken;
    extern std::unique_ptr<PointLight> pLight;
    extern std::unique_ptr<Ground> ground;
    extern std::unique_ptr<Player> player;
    extern std::unique_ptr<CubeMap> cubeMap;

    void Setup();
    void Dealloc();

    void Update();
    void Draw();

    // Interface to camera
    glm::mat4 GetViewMatrix();
    glm::vec3 GetCameraPosition();
}