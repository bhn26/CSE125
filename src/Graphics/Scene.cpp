#include "Scene.h"

#include "Objects/Cube.h"
#include "Camera.h"

#include <algorithm>

std::unique_ptr<Cube> Scene::cube;
std::unique_ptr<Camera> Scene::camera;

void Scene::Setup()
{
    cube = std::unique_ptr<Cube>(new Cube);
    camera = std::unique_ptr<Camera>(new Camera);
}

void Scene::Dealloc()
{
}