#include "Scene.h"

#include "Graphics/ModelManager.h"
#include "Objects/CubeMap.h"
#include "Objects/Cube.h"
#include "Objects/Egg.h"
#include "Objects/Chicken.h"
#include "Objects/InstanceObject.h"
#include "Objects/StaticObject.h"
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
Scene::Scene() : camera(std::unique_ptr<Camera>(nullptr)), pLight(nullptr),
    dLight(nullptr), player(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
void Scene::Setup()
{
    entities.clear();

    InitializeUBOs();
    InitializeFBO();

    //camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 9.0f, -15.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -25.0f));
    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(0.0f, 9.0f, -15.0f)));
    pLight = std::unique_ptr<PointLight>(new PointLight(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    dLight = std::unique_ptr<DirectionalLight>(new DirectionalLight(glm::vec3(0.5, -sqrt(3)/2.0f, 0.0f)));

    glm::vec3 pos = glm::vec3(50.0f, 100.0f, 0.0f);
    //lightSpaceMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f) *
    //    glm::lookAt(pos, pos + dLight->_direction, glm::vec3(0.0f, 1.0f, 0.0));
    lightSpaceMatrix = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 100.0f) *
        glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0));

    depthShader = ShaderManager::GetShader("Depth_Map");

	std::unique_ptr<StaticObject> map = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Map")));

	// Barn
    std::unique_ptr<StaticObject> barn = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Barn")));

	// Tractors
    std::unique_ptr<StaticObject> red_tractor = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Tractor_Red")));
	red_tractor->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//red_tractor->Translate(glm::vec3(17.0f, 0.0f, -13.0f));

    std::unique_ptr<StaticObject> green_tractor = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Tractor_Green")));
	green_tractor->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//green_tractor->Translate(glm::vec3(34.0f, 0.0f, 13.0f));

    std::unique_ptr<StaticObject> orange_tractor = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Tractor_Orange")));
	orange_tractor->Rotate(90.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	//orange_tractor->Translate(glm::vec3(-17.0f, 0.0f, -13.0f));

	// Silo
    std::unique_ptr<StaticObject> silo = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Silo")));
	//silo->Translate(glm::vec3(-28.0f, 0.0f, -4.0f));

	// Construction
	std::unique_ptr<StaticObject> construction_site = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Construction_Site")));
    construction_site->SetScale(0.15f);
    construction_site->Rotate(270.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    construction_site->Translate(glm::vec3(2.0f, 0.0f, -216.0f));

	// Maze
	std::unique_ptr<StaticObject> maze = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Maze")));

	// Bench
    std::unique_ptr<StaticObject> bench = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Bench")));
    bench->SetScale(4.0f);
	bench->Translate(glm::vec3(40.0f, 3.2f, 0.0f));
	bench->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Pumpkin
    // TODO NO IDEA WHY PUMPKIN HACK WORKS HERE
    ModelManager::Instance()->AddModelToLoad("Pumpkin");
    ModelManager::Instance()->LoadModels();
    std::unique_ptr<StaticObject> pumpkinObj = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Pumpkin")));

	// Rocks
    std::unique_ptr<StaticObject> rocks = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Rocks")));
	rocks->Translate(glm::vec3(28.0f, 0.2f, -20.0f));

	// Stump
    std::unique_ptr<StaticObject> stump = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Stump")));
	stump->Translate(glm::vec3(-28.0f, 0.2f, -20.0f));

	// Ground
    std::unique_ptr<StaticObject> ground = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Ground")));
	//ground->Translate(glm::vec3(0.0f, 100.4f, 0.0f));

	// Seed
    std::unique_ptr<StaticObject> seed = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Pumpkin_Seed")));
    seed->SetScale(0.5f);
	seed->Translate(glm::vec3(0.0f, 1.0f, 0.0f));

	//// Boat
 //   std::unique_ptr<StaticObject> boat = std::unique_ptr<StaticObject>(new StaticObject("assets/map/new_models/boat.obj"));
 //   boat->Scale(100.0f);
 //   boat->Translate(glm::vec3(0.0f, 50.4f, 40.0f));

    std::unique_ptr<StaticObject> windmill = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Windmill")));
    windmill->SetScale(1/4.0f);
    windmill->Translate(glm::vec3(75.0f, 0.0f, 75.0f));
    

	grass = std::unique_ptr<InstanceObject>(new InstanceObject(ModelManager::GetModel("Grass"), 10000, 1.0f));
	pumpkin = std::unique_ptr<InstanceObject>(new InstanceObject(ModelManager::GetModel("Pumpkin"), 40, 10.0f));
	// Pumpkins
	//pumpkin = std::unique_ptr<InstanceObject>(new InstanceObject("assets/map/objects/pumpkin.obj", 10000));
	//pumpkin->Translate(glm::vec3(0.0f, 0.0f, -1.0f));

    cubeMap = std::unique_ptr<CubeMap>(new CubeMap);
    cubeMap->LoadCubeMap();
    cubeMap->GetShader() = ShaderManager::GetShader("CubeMap");
    grass->GetShader() = ShaderManager::GetShader("Instancing");

//	static_objects.push_back(std::move(map));
	//static_objects.push_back(std::move(boat));
	static_objects.push_back(std::move(barn));
	static_objects.push_back(std::move(ground));
	static_objects.push_back(std::move(red_tractor));
	static_objects.push_back(std::move(green_tractor));
	static_objects.push_back(std::move(orange_tractor));
	static_objects.push_back(std::move(construction_site));
	static_objects.push_back(std::move(windmill));
	static_objects.push_back(std::move(maze));
	static_objects.push_back(std::move(silo));
	static_objects.push_back(std::move(rocks));
	static_objects.push_back(std::move(bench));
	//static_objects.push_back(std::move(pumpkinObj));
	static_objects.push_back(std::move(seed));
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
void Scene::InitializeFBO()
{
    if (this->depthMapFBO)
        return;

    glGenFramebuffers(1, &this->depthMapFBO);

    // Generate the Depth Map texture
    glGenTextures(1, &this->depthMap);
    glBindTexture(GL_TEXTURE_2D, this->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_DEPTH_WIDTH, SHADOW_DEPTH_HEIGHT,
        0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Bind depth map texture to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

////////////////////////////////////////////////////////////////////////////////
void Scene::SetProjectionUBO()
{
    glm::mat4 projection = glm::perspective(45.0f, (float)Window::width/(float)Window::height, 0.1f, 1000.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

////////////////////////////////////////////////////////////////////////////////
void Scene::ConfigureShaderAndMatrices()
{
    depthShader->Use();
    glUniformMatrix4fv(depthShader->GetUniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(LightSpaceMatrix()));
}

////////////////////////////////////////////////////////////////////////////////
void Scene::RenderDepthMap()
{
    renderingDepthMap = true;
    glViewport(0, 0, SHADOW_DEPTH_WIDTH, SHADOW_DEPTH_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ConfigureShaderAndMatrices();
        RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    renderingDepthMap = false;
}

////////////////////////////////////////////////////////////////////////////////
void Scene::DrawDepthMap()
{
    glViewport(0, 0, Window::width/3, Window::height/3);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    std::shared_ptr<Shader>& shader = ShaderManager::Instance()->GetShader("Depth_Draw");
    shader->Use();
    glUniform1f(shader->GetUniform("near_plane"), 0.1f);
    glUniform1f(shader->GetUniform("far_plane"), 1000.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    RenderQuad();
}

////////////////////////////////////////////////////////////////////////////////
void Scene::RenderScene()
{
    // Clear the color and depth buffers
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!IsRenderingDepth())
        glViewport(0, 0, Window::width, Window::height);
    SetViewUBO();
    cubeMap->Draw();
    grass->Draw();
    //pumpkin->Draw();

    //if (renderingDepthMap)
    //{
    //    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //    ConfigureShaderAndMatrices();
    //}

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
void Scene::RenderQuad()
{
    static GLuint quadVAO = 0;
    static GLuint quadVBO = 0;
    if (quadVAO == 0)       // Initialize
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
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

    RenderDepthMap();
    DrawDepthMap();
    RenderScene();
}


////////////////////////////////////////////////////////////////////////////////
glm::mat4 Scene::GetViewMatrix()
{
    if (renderingDepthMap)
    {
    }
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
		egg = std::unique_ptr<Egg>(new Egg(p.x, p.y, p.z, "Easter_Egg"));
		egg->SetColor(glm::vec3(0.27f, 0.16f, 0.0f));
		egg->GetShader() = ShaderManager::GetShader("Model");
		egg->SetClassId(p.cid);
		egg->SetObjId(p.oid);
		AddEntity(p.cid, p.oid, std::move(egg));
		break;
    case ClassId::BULLET:
    {
        //std::unique_ptr<StaticObject> bullet = std::unique_ptr<StaticObject>(new StaticObject("assets/weapons/pumpkinseed.obj"));
        std::unique_ptr<StaticObject> bullet = std::unique_ptr<StaticObject>(new StaticObject(ModelManager::GetModel("Pumpkin_Seed")));
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

