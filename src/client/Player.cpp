#include "Player.h"

#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Camera.h"
#include "Graphics/Objects/Chicken.h"
#include "Graphics/Shader.h"
#include "Graphics/Scene.h"
#include "Graphics/PointLight.h"
#include "Graphics/Model.h"
#include "Graphics/Animation/AnimatedModel.h"
#include "client/ClientGame.h"
#include "Basic/Utils.h"

Player::Player(float x, float y, float z, float rotW, float rotX, float rotY, float rotZ) : Entity(x,y,z), camAngle(0.0f), modelFile("assets/chickens/objects/chicken.obj")
{
    //for (int col = 0; col < 3; col++)
    //    for (int row = 0; row < 3; row++)
    //        toWorld[col][row] *= 0.01;
    model = std::unique_ptr<Model>(new Model(modelFile.c_str()));
    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(-1.5f, 4.5f, -7.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -15.0f));
    //camera = std::unique_ptr<Camera>(new Camera(glm::vec3(1.5f, 4.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -15.0f));
    Entity::RotateTo(rotW, rotX, rotY, rotZ);

    //m_model = std::unique_ptr<Animation::AnimatedModel>(new Animation::AnimatedModel);
    //m_animNames["dance"] = m_model->FBXLoadClean("assets/chickens/chicken_dance.fbx", true);
    //m_animNames["walk"] = m_model->FBXLoadClean("assets/chickens/chicken_walk.fbx", true);
    //m_animNames["peck"] = m_model->FBXLoadClean("assets/chickens/chicken_peck.fbx", false);
    //m_animNames["jump"] = m_model->FBXLoadClean("assets/chickens/chicken_jump.fbx", false);

    //m_model->RegisterListener(this);
}

Player::Player(int client_id) : Player()
{
    id = client_id;
}

Player::~Player()
{
}

void Player::SetModelFile(std::string fileName){
    modelFile = fileName;
    model = std::unique_ptr<Model>(new Model(modelFile.c_str()));
    //m_model = std::unique_ptr<Animation::AnimatedModel>(new Animation::AnimatedModel(fileName));
}

void Player::Draw() const
{
    shader->Use();

    GLint viewLoc = shader->GetUniform("view");
    GLint modelLocation = shader->GetUniform("model");
    GLint normalMatrixLoc = shader->GetUniform("normalMatrix");
    GLint projectionLocation = shader->GetUniform("projection");
    GLint lightColorLoc = shader->GetUniform("lightColor");
    GLint lightPosLoc = shader->GetUniform("lightPos");
    GLint viewPosLoc = shader->GetUniform("viewPos");

    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(normalMatrixLoc, 1, false, glm::value_ptr(GetNormalMatrix()));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));

    glUniform3fv(lightColorLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->color));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(Scene::Instance()->GetPointLight()->position));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(Scene::Instance()->GetCameraPosition()));

    model->Draw(shader.get());
    //SkinningTechnique* skinTechnique = m_model->GetMesh().GetSkinningTechnique();
    //skinTechnique->Enable(); // use shader

    //skinTechnique->SetEyeWorldPos(Scene::Instance()->GetCameraPosition());
    //skinTechnique->SetWVP(Scene::Instance()->GetPerspectiveMatrix() * Scene::Instance()->GetViewMatrix() * toWorld);
    //skinTechnique->SetWorldMatrix(toWorld);

    //m_model->Draw();
}

void Player::MoveTo(float x, float y, float z)
{
    Entity::MoveTo(x, y, z);
    ChangeState(STATE::WALK);
}

// Process movement
void Player::ProcessKeyboard(DIRECTION direction, GLfloat deltaTime)
{
    // Update the translation component of the world matrix
    if (direction == D_FORWARD)
        this->toWorld[3] += deltaTime * toWorld[2];
    if (direction == D_BACKWARD)
        this->toWorld[3] -= deltaTime * toWorld[2];
    if (direction == D_LEFT)
        this->toWorld[3] += deltaTime * toWorld[0];
    if (direction == D_RIGHT)
        this->toWorld[3] -= deltaTime * toWorld[0];
    if (direction == D_UP)
        this->toWorld[3] += deltaTime * toWorld[1];
    if (direction == D_DOWN)
        this->toWorld[3] -= deltaTime * toWorld[1];
}
int tick = 0;
// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Player::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= 0.25f;
    yoffset *= 0.1f;

    // Update Front, Right and Up Vectors using the updated Eular angles

    this->toWorld = this->toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(-xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
                                  //* glm::rotate(glm::mat4(1.0f), glm::radians(-yoffset), glm::vec3(1.0f, 0.0f, 0.0f));
    camAngle += glm::radians(yoffset);
    const static float pi2 = glm::pi<float>()/2;
    camAngle = (camAngle > pi2) ? pi2 : ((camAngle < -pi2) ? -pi2 : camAngle);
	if (++tick % 10 == 0)
	{
		ClientGame::instance()->sendRotationPacket();
		tick = 0;
	}
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Player::ProcessMouseScroll(GLfloat yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

glm::vec3 Player::CameraPosition() const
{
    return glm::vec3(toWorld * glm::rotate(glm::mat4(1.0f), camAngle, glm::vec3(-1.0f, 0.0f, 0.0f)) * glm::vec4(camera->Position(), 1.0f));
}

glm::mat4 Player::GetViewMatrix() const
{
    return camera->GetViewMatrix() * glm::rotate(glm::mat4(1.0f), camAngle, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::inverse(toWorld);
}

glm::mat4 Player::GetPerspectiveMatrix() const
{
    return camera->GetPerspectiveMatrix();
}

glm::mat3 Player::GetNormalMatrix() const
{
    return glm::mat3(glm::transpose(glm::inverse(toWorld)));
}

void Player::ChangeState(STATE state)
{
    SetState(state);
    //switch (state)
    //{
    //    case STATE::IDLE:
    //        m_model->Reset();
    //        break;
    //    case STATE::WALK:
    //        m_model->PlayAnimation(m_animNames["walk"]);
    //        m_lastTime_t = Utils::CurrentTime();
    //        m_lastPos_t = Position();
    //        break;
    //    case STATE::JUMP:
    //        m_model->PlayAnimation(m_animNames["jump"]);
    //        break;
    //    case STATE::PECK:
    //        m_model->PlayAnimation(m_animNames["peck"]);
    //        break;
    //}
}

// AnimationPlayer::Listener
void Player::OnFinish()
{
    SetState(STATE::IDLE);
}

void Player::Update()
{
    //if (m_state == STATE::WALK)
    //{
    //    if (Utils::CurrentTime() - m_lastTime_t > 1.0f)
    //    {
    //        if (m_lastPos_t == Position())
    //            ChangeState(STATE::IDLE);
    //    }
    //}
    //m_model->Update();
}

void Player::Spawn(float x, float y, float z)
{
}
