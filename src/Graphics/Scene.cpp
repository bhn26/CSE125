#include "Scene.h"

#include "Objects/Cube.h"
#include "Objects/Chicken.h"
#include "Camera.h"
#include "PointLight.h"

#include <algorithm>

std::unique_ptr<Cube> Scene::cube;
std::unique_ptr<Camera> Scene::camera;
std::unique_ptr<Chicken> Scene::chicken;
std::unique_ptr<PointLight> Scene::pLight;

void Scene::Setup()
{
    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 0.0f, 3.0f)));
    pLight = std::unique_ptr<PointLight>(new PointLight(glm::vec3(3.0f, 2.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

    cube = std::unique_ptr<Cube>(new Cube);
    cube->shader.SetShaders("src/Graphics/Shaders/basic_shader.vert",
                            "src/Graphics/Shaders/basic_shader.frag");
    chicken = std::unique_ptr<Chicken>(new Chicken);
}

void Scene::Dealloc()
{
}

void Scene::Update(bool rotX, bool rotY, bool rotZ)
{
    cube->Update();
    chicken->Update(rotX, rotY, rotZ);
}

void Scene::Draw()
{
    chicken->Draw(camera.get());
}