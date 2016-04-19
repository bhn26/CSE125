#include "Scene.h"

#include "Objects/Cube.h"
#include "Objects/Chicken.h"
#include "Objects/Ground.h"
#include "Camera.h"
#include "PointLight.h"

#include <memory>
#include <algorithm>

#include "Objects/ChickenAnim.h"

std::unique_ptr<Cube> Scene::cube;
std::unique_ptr<Camera> Scene::camera;
std::unique_ptr<Chicken> Scene::chicken;
std::unique_ptr<PointLight> Scene::pLight;
std::unique_ptr<Ground> Scene::ground;

std::unique_ptr<ChickenAnim> Scene::chicken_anim;

void Scene::Setup()
{
    std::shared_ptr<Shader> basicShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
    std::shared_ptr<Shader> diffuseShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/diffuse.frag");
    std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 4.0f, 4.0f));
    pLight = std::make_unique<PointLight>(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    chicken = std::unique_ptr<Chicken>(new Chicken);
    ground = std::unique_ptr<Ground>(new Ground);

    cube = std::make_unique<Cube>();
    cube->shader = basicShader;
    ground->shader = diffuseShader;
    chicken->shader = modelShader;
    
    chicken_anim = std::unique_ptr<ChickenAnim>(new ChickenAnim);
}

void Scene::Dealloc()
{
}

void Scene::Update(bool rotX, bool rotY, bool rotZ)
{
    //cube->Update();
    //chicken->Update(rotX, rotY, rotZ);
    chicken_anim->Update();
}

void Scene::Draw()
{
    //chicken->Draw(camera.get());
    ground->Draw(camera->GetViewMatrix());
    //cube->Draw(camera->GetViewMatrix());
    
    chicken_anim->Draw(camera.get());
}