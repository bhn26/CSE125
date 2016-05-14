#include "Scene.h"

#include "Objects/CubeMap.h"
#include "Objects/Cube.h"
#include "Objects/Chicken.h"
#include "Objects/Ground.h"
#include "Camera.h"
#include "PointLight.h"
#include "Objects/StaticObject.h"
#include "Objects/Entity.h"
#include "../client/Player.h"
#include "../client/ClientGame.h"
#include <algorithm>
#include <vector>

Scene::Scene() : camera(std::unique_ptr<Camera>(nullptr)), pLight(std::unique_ptr<PointLight>(nullptr)),
    player(nullptr), entities(std::vector<std::unique_ptr<Entity>>()), players(std::vector<std::shared_ptr<Player>>())
{
}
const int Scene::WIDTH = 100;
const int Scene::HEIGHT = 100;

void Scene::Setup()
{
    entities.clear();
    std::shared_ptr<Shader> basicShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
    std::shared_ptr<Shader> diffuseShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/diffuse.frag");
    std::shared_ptr<Shader> cubeMapShader = std::make_shared<Shader>("src/Graphics/Shaders/cubemap.vert", "src/Graphics/Shaders/cubemap.frag");

    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 9.0f, -15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -25.0f));
    pLight = std::unique_ptr<PointLight>(new PointLight(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

	// Barn
	std::unique_ptr<StaticObject> barn = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/barn.obj"));
	barn->Scale(8.0f);
	barn->Translate(glm::vec3(0.0f, 0.0f, 10.0f));

	// Tractor
	std::unique_ptr<StaticObject> tractor = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/tractor.obj"));
	tractor->Scale(7.0f);
	tractor->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	tractor->Translate(glm::vec3(17.0f, 0.0f, -13.0f));

	// Silo
	std::unique_ptr<StaticObject> silo = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/silo.obj"));
	silo->Scale(3.0f);
	silo->Translate(glm::vec3(-13.0f, 0.0f, -4.0f));

	// Pumpkin
	std::unique_ptr<StaticObject> pumpkin = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/pumpkin.obj"));
	pumpkin->Translate(glm::vec3(0.0f, 0.0f, -1.0f));

	// Bench
	std::unique_ptr<StaticObject> bench = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/wood_bench.obj"));
	bench->Scale(0.01f);
	bench->Translate(glm::vec3(0.0f, 0.0f, -60.0f));

    std::unique_ptr<Ground> ground = std::unique_ptr<Ground>(new Ground);
    std::unique_ptr<Cube> cube = std::unique_ptr<Cube>(new Cube);
    std::unique_ptr<CubeMap> cubeMap = std::unique_ptr<CubeMap>(new CubeMap);
    cubeMap->LoadCubeMap();

   // cube->GetShader() = basicShader;
    ground->GetShader() = diffuseShader;
    cubeMap->GetShader() = cubeMapShader;

	entities.push_back(std::move(barn));
	entities.push_back(std::move(tractor));
	entities.push_back(std::move(silo));
    entities.push_back(std::move(ground));
	entities.push_back(std::move(bench));
   // entities.push_back(std::move(cube));
    entities.push_back(std::move(cubeMap));
}

void Scene::AddPlayer(int client_id) {
	//TODO - add client_id field to player
	std::shared_ptr<Player> new_player = std::shared_ptr<Player>(new Player(client_id));

	std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");
	new_player->GetShader() = modelShader;

	players.push_back(new_player);

	if (client_id == ClientGame::GetClientId()) {
		printf("set main player to %d\n", client_id);
		player = new_player; // set your player
	}
}

void Scene::Update()
{
    for (std::unique_ptr<Entity>& entity : entities)
        entity->Update();
}

void Scene::Draw()
{
    for (std::unique_ptr<Entity>& entity : entities)
        entity->Draw();

    // Redrawing players??
	for (int i = 0; i < players.size(); i++) {
		players.at(i)->Draw();
	}
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

glm::mat4 Scene::GetPerspectiveMatrix()
{
    return camera->GetPerspectiveMatrix();
}