#pragma once
#include <memory>

class Camera;
class Cube;

namespace Scene
{
    extern std::unique_ptr<Camera> camera;
    extern std::unique_ptr<Cube> cube;

    void Setup();
    void Dealloc();
}