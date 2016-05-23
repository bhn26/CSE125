#include "Scene.h"

#include "Objects/CubeMap.h"
#include "Objects/Cube.h"
#include "Objects/Egg.h"
#include "Objects/Chicken.h"
#include "Camera.h"
#include "PointLight.h"
#include "Objects/Entity.h"
#include "../client/Player.h"
#include "../client/ClientGame.h"
#include "../server/engine/ObjectId.h"
#include <algorithm>
#include <vector>
#include <map>

Scene::Scene() : camera(std::unique_ptr<Camera>(nullptr)), pLight(std::unique_ptr<PointLight>(nullptr)),
    player(nullptr), players(std::vector<std::shared_ptr<Player>>())
{
}
const int Scene::WIDTH = 100;
const int Scene::HEIGHT = 100;

void Scene::Setup()
{
    entities.clear();

	instanceShader = std::make_shared<Shader>("src/Graphics/Shaders/instancing.vert", "src/Graphics/Shaders/instancing.frag");
    basicShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
    diffuseShader = std::make_shared<Shader>("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/diffuse.frag");
    modelShader = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");
    cubeMapShader = std::make_shared<Shader>("src/Graphics/Shaders/cubemap.vert", "src/Graphics/Shaders/cubemap.frag");

    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 9.0f, -15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -25.0f));
    pLight = std::unique_ptr<PointLight>(new PointLight(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

	// Barn
	std::unique_ptr<StaticObject> barn = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/barn2.obj"));
	barn->Scale(17.0f);
	barn->Translate(glm::vec3(0.0f, 0.0f, 20.0f));

	// Tractor
	std::unique_ptr<StaticObject> tractor = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/tractor.obj"));
	tractor->Scale(12.0f);
	tractor->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	tractor->Translate(glm::vec3(17.0f, 0.0f, -13.0f));

	// Silo
	std::unique_ptr<StaticObject> silo = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/silo.obj"));
	silo->Scale(10.0f);
	silo->Translate(glm::vec3(-28.0f, 0.0f, -4.0f));

	// Pumpkin
	std::unique_ptr<StaticObject> pumpkin = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/pumpkin.obj"));
	pumpkin->Translate(glm::vec3(0.0f, 0.0f, -1.0f));

	// Bench
	std::unique_ptr<StaticObject> bench = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/bench3.obj"));
	bench->Scale(4.0f);
	bench->Translate(glm::vec3(40.0f, 3.2f, 0.0f));
	bench->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Rocks
	std::unique_ptr<StaticObject> rocks = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/rocks.obj"));
	rocks->Scale(4.0f);
	rocks->Translate(glm::vec3(28.0f, 0.2f, -20.0f));

	// Rocks
	std::unique_ptr<StaticObject> stump = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/stump.obj"));
	stump->Scale(4.0f);
	stump->Translate(glm::vec3(-28.0f, 0.2f, -20.0f));

	// Ground
	std::unique_ptr<StaticObject> ground = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/ground.obj"));
	ground->Scale(4.0f);
	ground->Translate(glm::vec3(0.0f, 27.3f, 0.0f));


	//std::unique_ptr<Ground> ground = std::unique_ptr<Ground>(new Ground);
	//std::unique_ptr<Cube> cube = std::unique_ptr<Cube>(new Cube);
	grass = std::unique_ptr<Grass>(new Grass);
    cubeMap = std::unique_ptr<CubeMap>(new CubeMap);
    cubeMap->LoadCubeMap();

	grass->GetShader() = instanceShader;
    cubeMap->GetShader() = cubeMapShader;

	//static_objects.push_back(std::move(grass));
	static_objects.push_back(std::move(barn));
	static_objects.push_back(std::move(tractor));
	static_objects.push_back(std::move(silo));
	static_objects.push_back(std::move(rocks));
	static_objects.push_back(std::move(bench));
	static_objects.push_back(std::move(ground));
}

void Scene::AddPlayer(int client_id) {
	//TODO - add client_id field to player
	std::shared_ptr<Player> new_player = std::shared_ptr<Player>(new Player(client_id));

	std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("src/Graphics/Shaders/model_loading.vert", "src/Graphics/Shaders/model_loading.frag");
	new_player->GetShader() = modelShader;

	players.push_back(new_player);

	if (client_id == ClientGame::GetClientId()) {
		printf("set main player to %d\n", client_id);
//		player = new_player; // set your player
	}
}

void Scene::Update()
{
	cubeMap->Update();
	//ground->Update();
	for (auto& const entity : entities)
		entity.second->Update();
}

void Scene::Draw()
{
	cubeMap->Draw();
	grass->Draw();

	for (auto& const obj : static_objects)
		obj->Draw();

	//ground->Draw();
	for (auto& const entity : entities)
        entity.second->Draw();

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

void Scene::AddEntity(int cid, int oid, std::unique_ptr<Entity> ent)
{
	std::pair<int, int> p = std::pair<int, int>(cid, oid);
	//entities.insert(std::make_pair(p, std::move(ent)));
	entities[p] = std::move(ent);
}

void Scene::AddEntity(int cid, int oid, float x, float y, float z, float rotw, float rotx, float roty, float rotz)
{
	std::unique_ptr<Player> player;
	std::unique_ptr<Egg> egg;

	switch (cid) {
	case ClassId::PLAYER:
		player = std::unique_ptr<Player>(new Player(x,y,z,rotw,rotx,roty,rotz));
		player->SetModelFile("assets/chickens/objects/pinocchio_chicken.obj");
		player->Spawn(x, y, z);
		player->GetShader() = modelShader;
		//player->RotateTo(rotw, rotx, roty, rotz);
		// set main player if the oid matches
		if (oid == ClientGame::instance()->GetClientId())
			Scene::player = player.get();
		//players.push_back(player);

		AddEntity(cid, oid, std::move(player));
		break;
	case ClassId::FLAG:
		egg = std::unique_ptr<Egg>(new Egg(x,y,z));
		egg->GetShader() = modelShader;
		AddEntity(cid, oid, std::move(egg));
		break;
	default:
		break;
	}
	
}

void Scene::RemoveEntity(int cid, int oid)
{
	int removed = entities.erase(std::make_pair(cid, oid));
}

std::unique_ptr<Entity>& Scene::GetEntity(int cid, int oid)
{
	std::pair<int, int> p = std::pair<int, int>(cid, oid);
	return entities.find(p)->second;
}