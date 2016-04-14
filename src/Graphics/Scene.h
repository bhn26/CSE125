#pragma once
#include <memory>

class Camera;
class Cube;
class Chicken;
class Ground;
struct PointLight;

namespace Scene
{
    extern std::unique_ptr<Camera> camera;
    extern std::unique_ptr<Cube> cube;
    extern std::unique_ptr<Chicken> chicken;
    extern std::unique_ptr<PointLight> pLight;
    extern std::unique_ptr<Ground> ground;

    void Setup();
    void Dealloc();

    void Update();
    void Draw();
}