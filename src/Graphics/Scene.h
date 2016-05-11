#pragma once
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <vector>

#include "Objects/Entity.h"

class Camera;
class Player;

struct PointLight;
class ChickenAnim;

class Scene
{
    std::unique_ptr<Camera> camera;
    std::unique_ptr<PointLight> pLight;
    std::shared_ptr<Player> player;

    static const int WIDTH;
    static const int HEIGHT;

    std::vector<std::unique_ptr<Entity>> entities;
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

    void AddPlayer(int client_id);
    void Update();

    void Draw();

    // Interface to camera
    glm::mat4 GetViewMatrix();
    glm::vec3 GetCameraPosition();
    glm::mat4 GetPerspectiveMatrix();

    std::unique_ptr<PointLight>& GetPointLight() { return pLight; }
    std::shared_ptr<Player>& GetPlayer() { return player; }
    std::vector<std::shared_ptr<Player>>& GetPlayers() { return players; };
    void ClearPlayers() { players.clear(); };
};
