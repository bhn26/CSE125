#pragma once
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <vector>
#include <map>

#include "Objects/Entity.h"
#include "Objects/CubeMap.h"

class Camera;
class Player;

struct PointLight;

class Scene
{
    std::unique_ptr<Camera> camera;
    std::unique_ptr<PointLight> pLight;
	std::unique_ptr<CubeMap> cubeMap;
    Player* player;

    static const int WIDTH;
    static const int HEIGHT;

	std::map<std::pair<int, int>, std::unique_ptr<Entity> > entities;
    std::vector<std::shared_ptr<Player>> players;

    Scene();

    void Setup();

public:
    static Scene* Instance()
    {
        static Scene* instance = new Scene();
        return instance;
    }

    static void Initialize() { Instance()->Setup(); }

	void AddEntity(int cid, int oid, std::unique_ptr<Entity> ent);
	void RemoveEntity(int cid, int oid);

	void AddPlayer(int client_id);
    void Update();
    void Draw();

    // Interface to camera
	glm::mat4 GetViewMatrix();
    glm::vec3 GetCameraPosition();
    glm::mat4 GetPerspectiveMatrix();

    std::unique_ptr<PointLight>& GetPointLight() { return pLight; }
    Player*& GetPlayer() { return player; }
};
