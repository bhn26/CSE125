#pragma once
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <map>

#include "Objects/Entity.h"
#include "Objects/Ground.h"
#include "Objects/Grass.h"
#include "ShaderManager.h"
#include "client/SpriteRenderer.h"
#include "network/GameData.h"


class Camera;
class Player;
class Shader;
class CubeMap;
class InstanceObject;
class StaticObject;

struct PointLight;
struct DirectionalLight;

class Scene
{
    enum UBOIndex
    {
        Matrices = 0,
        Lights = 1,
    };

    const GLuint SHADOW_DEPTH_WIDTH = 1024, SHADOW_DEPTH_HEIGHT = 1024;
    unsigned int shadowMapIndex = 7;
    float lastTime;
    GLuint uboMatricesBuffer;
    GLuint depthMapFBO;
    GLuint depthMap;
    bool renderingDepthMap = false;
    std::shared_ptr<Shader> depthShader;
    glm::mat4 lightSpaceMatrix;

    std::unique_ptr<Camera> camera;
    std::unique_ptr<PointLight> pLight;
    std::unique_ptr<DirectionalLight> dLight;
	std::unique_ptr<CubeMap> cubeMap;
	std::unique_ptr<Ground> ground;
	std::unique_ptr<InstanceObject> grass;
	std::unique_ptr<InstanceObject> pumpkin;

    Player* player;

    static const int WIDTH;
    static const int HEIGHT;

	std::map<std::pair<int, int>, std::unique_ptr<Entity> > entities;
	std::vector<std::unique_ptr<StaticObject> > static_objects;

    Scene();
    void Setup();
    void InitializeUBOs();
    void InitializeFBO();
    void SetViewUBO();
    void SetProjectionUBO();
    void ConfigureShaderAndMatrices();
    void RenderDepthMap();
    void DrawDepthMap();
    void RenderScene();
    void RenderQuad();

public:
    static SpriteRenderer * sprite_renderer;

    static Scene* Instance()
    {
        static Scene* instance = new Scene();
        return instance;
    }

    static void Initialize() { Instance()->Setup(); }

	void AddEntity(int cid, int oid, std::unique_ptr<Entity> ent);
	void AddEntity(PosInfo p);
	void RemoveEntity(int cid, int oid);
	std::unique_ptr<Entity>& GetEntity(int cid, int oid);

    void Update();

    void Draw();

    // Interface to camera
    glm::mat4 GetViewMatrix();
    glm::vec3 GetCameraPosition();
    glm::mat4 GetPerspectiveMatrix();

    std::unique_ptr<PointLight>& GetPointLight() { return pLight; }
	Player*& GetPlayer() { return player; }
	StaticObject* GetStaticObject(int i) { return static_objects[i].get(); }
    DirectionalLight* GetDirectionalLight() { return dLight.get(); }
    int NumberOfStaticObjects() { return static_objects.size(); }

    bool IsRenderingDepth() const { return renderingDepthMap; }
    std::shared_ptr<Shader>& GetDepthShader() { return depthShader; }

    unsigned int ShadowMapIndex() const { return shadowMapIndex; }
    GLuint DepthMap() const { return depthMap; }
    const glm::mat4& LightSpaceMatrix()
    {
        //lightSpaceMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f) * GetViewMatrix();
        lightSpaceMatrix = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 1.0f, 500.0f) * GetViewMatrix();
        return lightSpaceMatrix;
    }

private:
    // helpers 
    static glm::vec2 Get2D(glm::vec3 coords, glm::mat4 view, glm::mat4 projection/*perspective matrix */, int width, int height);
};
