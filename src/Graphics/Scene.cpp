#include "Scene.h"

#include "Objects/CubeMap.h"
#include "Objects/Cube.h"
#include "Objects/Egg.h"
#include "Objects/Chicken.h"
#include "Camera.h"
#include "PointLight.h"

#include "Objects/ModelEntity.h"
#include "Objects/Entity.h"
#include "../client/Player.h"
#include "../client/ClientGame.h"

#include "../server/engine/ObjectId.h"

#include <algorithm>
#include <vector>
#include <map>

SpriteRenderer * Scene::sprite_renderer = new SpriteRenderer();
Scene::Scene() : camera(std::unique_ptr<Camera>(nullptr)), pLight(std::unique_ptr<PointLight>(nullptr)),
    player(nullptr)
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

	// Ground
	std::unique_ptr<StaticObject> ground = std::unique_ptr<StaticObject>(new StaticObject("assets/map/objects/ground.obj"));
	ground->Translate(glm::vec3(0.0f, 6.8f, 0.0f));
	//bench->Translate(glm::vec3(0.0f, 0.0f, -60.0f));

	grass = std::unique_ptr<Grass>(new Grass);
    //std::unique_ptr<Ground> ground = std::unique_ptr<Ground>(new Ground);
    //std::unique_ptr<Cube> cube = std::unique_ptr<Cube>(new Cube);
    cubeMap = std::unique_ptr<CubeMap>(new CubeMap);
    cubeMap->LoadCubeMap();

   // cube->GetShader() = basicShader;
	grass->GetShader() = instanceShader;
   // ground->GetShader() = diffuseShader;
    cubeMap->GetShader() = cubeMapShader;

	//static_objects.push_back(std::move(grass));
	static_objects.push_back(std::move(barn));
	static_objects.push_back(std::move(tractor));
	static_objects.push_back(std::move(silo));
	static_objects.push_back(std::move(ground));
  //  entities.push_back(std::move(ground));
	static_objects.push_back(std::move(bench));
   // entities.push_back(std::move(cube));
	//static_objects.push_back(std::move(cubeMap));

    /*std::unique_ptr<Player> player = std::unique_ptr<Player>(new Player);
	player->SetModelFile("assets/chickens/objects/pinocchio_chicken.obj");
    Scene::player = player.get();*/
    //ground = std::unique_ptr<Ground>(new Ground);
    //std::unique_ptr<Cube> cube = std::unique_ptr<Cube>(new Cube);
	/*std::unique_ptr<Egg> egg = std::unique_ptr<Egg>(new Egg(glm::vec3(10.0f, 3.0f, 10.0f)));
	egg->SetColor(glm::vec3(0.27f, 0.16f, 0.0f));*/

    //cube->GetShader() = basicShader;
	//egg->GetShader() = diffuseShader;
    //ground->GetShader() = diffuseShader;
    //player->GetShader() = modelShader;

    /*entities.push_back(std::move(ground));
    entities.push_back(std::move(player));
	entities.push_back(std::move(egg));
    //entities.push_back(std::move(cube)); // Don't add cube to scene
    entities.push_back(std::move(cubeMap));*/

}


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

void Scene::Draw()
{
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
    if (player) return player->GetPerspectiveMatrix();
    return camera->GetPerspectiveMatrix();
}

void Scene::AddEntity(int cid, int oid, std::unique_ptr<Entity> ent)
{
	std::pair<int, int> p = std::pair<int, int>(cid, oid);
	entities[p] = std::move(ent);
}

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
        player->Spawn(p.x, p.y, p.z);
		player->GetShader() = modelShader;
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
		egg->GetShader() = diffuseShader;
		egg->SetClassId(p.cid);
		egg->SetObjId(p.oid);
		AddEntity(p.cid, p.oid, std::move(egg));
		break;
    case ClassId::BULLET:
    {
        std::unique_ptr<ModelEntity> bullet = std::unique_ptr<ModelEntity>(new ModelEntity("assets/eggs/objects/egg.obj"));
        //bullet->GetShader() = modelShader;        // Set in ModelEntity
        AddEntity(p.cid, p.oid, std::move(bullet));
        break;
    }
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

