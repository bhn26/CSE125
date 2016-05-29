#include "Scene.h"

#include "Objects/CubeMap.h"
#include "Objects/Cube.h"
#include "Objects/Egg.h"
#include "Objects/Chicken.h"
#include "Camera.h"
#include "Lights.h"

#include "Objects/ModelEntity.h"
#include "Objects/Entity.h"
#include "client/Player.h"
#include "client/ClientGame.h"

#include "server/engine/ObjectId.h"

#include <algorithm>
#include <vector>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////////////////////
const int Scene::WIDTH = 100;
const int Scene::HEIGHT = 100;

////////////////////////////////////////////////////////////////////////////////
Scene::Scene() : camera(std::unique_ptr<Camera>(nullptr)), pLight(std::unique_ptr<PointLight>(nullptr)),
player(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
void Scene::Setup()
{
    entities.clear();

    InitializeUBOs();

    //camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 9.0f, -15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -25.0f));
    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 9.0f, -15.0f)));
    pLight = std::unique_ptr<PointLight>(new PointLight(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

	// Barn
	std::unique_ptr<StaticObject> barn = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/barn2.obj"));
	barn->Scale(17.0f);
	barn->Translate(glm::vec3(0.0f, 0.0f, 20.0f));

	// Tractors
	std::unique_ptr<StaticObject> red_tractor = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/tractors/red_tractor.obj"));
	red_tractor->Scale(12.0f);
	red_tractor->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	red_tractor->Translate(glm::vec3(17.0f, 0.0f, -13.0f));

	std::unique_ptr<StaticObject> green_tractor = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/tractors/green_tractor.obj"));
	green_tractor->Scale(12.0f);
	green_tractor->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	green_tractor->Translate(glm::vec3(34.0f, 0.0f, 13.0f));

	std::unique_ptr<StaticObject> orange_tractor = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/tractors/orange_tractor.obj"));
	orange_tractor->Scale(12.0f);
	orange_tractor->Rotate(90.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	orange_tractor->Translate(glm::vec3(-17.0f, 0.0f, -13.0f));

	// Silo
	std::unique_ptr<StaticObject> silo = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/silo.obj"));
	silo->Scale(10.0f);
	silo->Translate(glm::vec3(-28.0f, 0.0f, -4.0f));

	// Bench
	std::unique_ptr<StaticObject> bench = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/bench3.obj"));
	bench->Scale(4.0f);
	bench->Translate(glm::vec3(40.0f, 3.2f, 0.0f));
	bench->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Pumpkin
	std::unique_ptr<StaticObject> pumpkinObj = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/pumpkin.obj"));

	// Rocks
	std::unique_ptr<StaticObject> rocks = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/nature/rocks.obj"));
	rocks->Scale(4.0f);
	rocks->Translate(glm::vec3(28.0f, 0.2f, -20.0f));

	// Rocks
	std::unique_ptr<StaticObject> stump = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/nature/stump.obj"));
	stump->Scale(4.0f);
	stump->Translate(glm::vec3(-28.0f, 0.2f, -20.0f));

	// Ground
	std::unique_ptr<StaticObject> ground = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/nature/ground.obj"));
	ground->Scale(15.0f);
	ground->Translate(glm::vec3(0.0f, 100.4f, 0.0f));

	// Seed
	std::unique_ptr<StaticObject> seed = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/pumpkinseed.obj"));
	seed->Scale(0.5f);
	seed->Translate(glm::vec3(0.0f, 1.0f, 0.0f));

	// Boat
	//std::unique_ptr<StaticObject> boat = std::unique_ptr<StaticObject>(new StaticObject("assets/map/new_models/boat.obj"));
//	boat->Scale(15.0f);
//	boat->Translate(glm::vec3(0.0f, 100.4f, 0.0f));

	grass = std::unique_ptr<Grass>(new Grass);
	// Pumpkins
	//pumpkin = std::unique_ptr<InstanceObject>(new InstanceObject("assets/map/objects/pumpkin.obj", 10000));
	//pumpkin->Translate(glm::vec3(0.0f, 0.0f, -1.0f));

    cubeMap = std::unique_ptr<CubeMap>(new CubeMap);
    cubeMap->LoadCubeMap();

    grass->GetShader() = ShaderManager::GetShader("Instancing");
    cubeMap->GetShader() = ShaderManager::GetShader("CubeMap");

	//static_objects.push_back(std::move(boat));
	static_objects.push_back(std::move(barn));
	static_objects.push_back(std::move(red_tractor));
	static_objects.push_back(std::move(green_tractor));
	static_objects.push_back(std::move(orange_tractor));
	static_objects.push_back(std::move(silo));
	static_objects.push_back(std::move(rocks));
	static_objects.push_back(std::move(bench));
	static_objects.push_back(std::move(pumpkinObj));
	static_objects.push_back(std::move(seed));
	static_objects.push_back(std::move(ground));
	
}

////////////////////////////////////////////////////////////////////////////////
void Scene::InitializeUBOs()
{
    ShaderManager::Instance()->ApplyUBOToAllShaders("Matrices", UBOIndex::Matrices);

    glGenBuffers(1, &this->uboMatricesBuffer);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesBuffer);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, UBOIndex::Matrices, uboMatricesBuffer, 0, 2*sizeof(glm::mat4));
    //glBindBufferBase(GL_UNIFORM_BUFFER, UBOIndex::Matrices, uboMatricesBuffer);

    SetProjectionUBO();
}

////////////////////////////////////////////////////////////////////////////////
void Scene::SetViewUBO()
{
    if (GetPlayer())
    {
        glm::mat4 view = GetPlayer()->GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void Scene::SetProjectionUBO()
{
    glm::mat4 projection = glm::perspective(45.0f, (float)Window::width/(float)Window::height, 0.1f, 1000.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

////////////////////////////////////////////////////////////////////////////////
void Scene::Update()
{
    double nextTime = Utils::CurrentTime();
    float deltaTime = (float)(nextTime - lastTime);
    lastTime = nextTime;

	cubeMap->Update(deltaTime);
	//ground->Update();
	for (auto& const entity : entities)
		entity.second->Update(deltaTime);
}

////////////////////////////////////////////////////////////////////////////////
void Scene::Draw()
{
    SetViewUBO();
	cubeMap->Draw();
	grass->Draw();

	for (auto& const obj : static_objects)
		obj->Draw();

	//ground->Draw();
	for (auto& const entity : entities)
	{
		entity.second->Draw();
		//printf("entity ids are %d, %d\n", entity.second->GetClassId(), entity.second->GetObjId());
	}
}


////////////////////////////////////////////////////////////////////////////////
glm::mat4 Scene::GetViewMatrix()
{
    if (player) return player->GetViewMatrix();
    return camera->GetViewMatrix();
}

////////////////////////////////////////////////////////////////////////////////
glm::vec3 Scene::GetCameraPosition()
{
    if (player) return player->CameraPosition();
    return camera->Position();
}

////////////////////////////////////////////////////////////////////////////////
glm::mat4 Scene::GetPerspectiveMatrix()
{
    if (player) return player->GetPerspectiveMatrix();
    return camera->GetPerspectiveMatrix();
}

////////////////////////////////////////////////////////////////////////////////
void Scene::AddEntity(int cid, int oid, std::unique_ptr<Entity> ent)
{
	std::pair<int, int> p = std::pair<int, int>(cid, oid);
	entities[p] = std::move(ent);
}

////////////////////////////////////////////////////////////////////////////////
void Scene::AddEntity(PosInfo p)
{
	std::unique_ptr<Player> player;
	std::unique_ptr<Egg> egg;
	std::string skin_type;

	switch (p.cid) {
	case ClassId::PLAYER:
		player = std::unique_ptr<Player>(new Player(p.x, p.y, p.z, p.rotw, p.rotx, p.roty, p.rotz));
		if (p.skin == 0)
		{
			skin_type = "assets/chickens/objects/chicken.obj";
		}
		else if (p.skin == 1)
		{
			skin_type = "assets/chickens/objects/robot_chicken.obj";
		}
		else if (p.skin == 2)
		{
			skin_type = "assets/chickens/objects/pinocchio_chicken.obj";
		}
		player->SetModelFile(skin_type);
		player->GetShader() = ShaderManager::GetShader("Model");
		player->SetObjId(p.oid);
		player->SetClassId(p.cid);
		player->SetTeam(p.team_id);
		//player->RotateTo(rotw, rotx, roty, rotz);
		// set main player if the oid matches
		if (p.oid == ClientGame::instance()->GetClientId())
			Scene::player = player.get();
		//players.push_back(player);

		AddEntity(p.cid, p.oid, std::move(player));
		break;
	case ClassId::FLAG:
		egg = std::unique_ptr<Egg>(new Egg(p.x, p.y, p.z));
		egg->SetColor(glm::vec3(0.27f, 0.16f, 0.0f));
		egg->GetShader() = ShaderManager::GetShader("Model");
		egg->SetClassId(p.cid);
		egg->SetObjId(p.oid);
		AddEntity(p.cid, p.oid, std::move(egg));
		break;
    case ClassId::BULLET:
    {
        std::unique_ptr<StaticObject> bullet = std::unique_ptr<StaticObject>(new StaticObject("assets/weapons/pumpkinseed.obj"));
		bullet->Translate(glm::vec3(p.x, p.y, p.z));
		printf("creating a bullet at %f, %f, %f\n", p.x, p.y, p.z);
        //bullet->GetShader() = modelShader;        // Set in ModelEntity
        AddEntity(p.cid, p.oid, std::move(bullet));
        break;
    }
	default:
		break;
	}
	
}

////////////////////////////////////////////////////////////////////////////////
void Scene::RemoveEntity(int cid, int oid)
{
	int removed = entities.erase(std::make_pair(cid, oid));
}

////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Entity>& Scene::GetEntity(int cid, int oid)
{
	std::pair<int, int> p = std::pair<int, int>(cid, oid);
	return entities.find(p)->second;
}

////////////////////////////////////////////////////////////////////////////////
glm::vec2 Scene::Get2D(glm::vec3 coords, glm::mat4 view, glm::mat4 projection/*perspective matrix */, int width, int height)
{
	glm::mat4 viewProjectionMatrix = projection * view;
	
	//transform world to clipping coordinates
	glm::vec3 clipping = glm::normalize(glm::vec3(viewProjectionMatrix * glm::vec4(coords, 1.0f)));
	int winX = (int)std::round(((clipping.x + 1) / 2.0) * width);
	
	//we calculate -point3D.getY() because the screen Y axis is
	//oriented top->down
	int winY = (int)std::round(((1 - clipping.y) / 2.0) * height);
	return glm::vec2(winX, winY);
}

