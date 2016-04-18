#include "Scene.h"

#include "Objects/Cube.h"
#include "Objects/Chicken.h"
#include "Objects/Ground.h"
#include "Camera.h"
#include "PointLight.h"
#include "../Player.h"

#include <algorithm>

std::unique_ptr<Cube> Scene::cube;
std::unique_ptr<Camera> Scene::camera;
std::unique_ptr<Chicken> Scene::chicken;
std::unique_ptr<PointLight> Scene::pLight;
std::unique_ptr<Ground> Scene::ground;
std::unique_ptr<Player> Scene::player;

void Scene::Setup()
{
    std::shared_ptr<Shader> basicShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
    std::shared_ptr<Shader> diffuseShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/diffuse.frag");
    std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");

    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 9.0f, -15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -25.0f));
    pLight = std::unique_ptr<PointLight>(new PointLight(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    //chicken = std::unique_ptr<Chicken>(new Chicken);
    ground = std::unique_ptr<Ground>(new Ground);
    player = std::unique_ptr<Player>(new Player);
    cube = std::unique_ptr<Cube>(new Cube);

    cube->shader = basicShader;
    ground->shader = diffuseShader;
    player->shader = modelShader;
    //chicken->shader = modelShader;
}

void Scene::Dealloc()
{
}

void Scene::Update()
{
    cube->Update();
    chicken->Update();
}

void Scene::Draw()
{
    //chicken->Draw(camera.get());
    ground->Draw(player->GetViewMatrix());
    cube->Draw(camera->GetViewMatrix());
    player->Draw();
}


glm::mat4 Scene::GetViewMatrix()
{
    if (player) return player->GetViewMatrix();
    return camera->GetViewMatrix();
}

glm::vec3 Scene::GetCameraPosition()
{
    if (player) return player->CameraPosition();
    return camera->Position();
}