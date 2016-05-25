#include "Player.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Graphics/Camera.h"
#include "Graphics/Objects/Chicken.h"
#include "Graphics/Shader.h"
#include "Graphics/Scene.h"
#include "Graphics/PointLight.h"
#include "Graphics/Model.h"
#include "Graphics/Animation/AnimatedModel.h"
#include "client/ClientGame.h"
#include "Basic/Utils.h"

//// rendering text about the player
#include "client/Window.h"
#include "client/TextRenderer.h"

Player::Player(float x, float y, float z, float rotW, float rotX, float rotY, float rotZ) :
    Entity(x,y,z), camAngle(0.0f), modelFile("assets/chickens/objects/chicken.obj"), defaultCamFront(glm::normalize(glm::vec3(0.0f, -0.20f, 0.97f)))
{
    info_panel = new Texture(GL_TEXTURE_2D, "assets/ui/player_info_panel.png");

    SetRelativeCamPosition(glm::vec3(-1.5f, 4.5f, -7.0f));
    model = std::unique_ptr<Model>(new Model(modelFile.c_str()));
    //camera = std::unique_ptr<Camera>(new Camera(Position() + relativeCamPosition, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -15.0f));
    camera = std::unique_ptr<Camera>(new Camera(Position() + relativeCamPosition));
    Entity::RotateTo(rotW, rotX, rotY, rotZ);

    //for (int col = 0; col < 3; col++)
    //    for (int row = 0; row < 3; row++)
    //        toWorld[col][row] *= 0.01;

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
    delete info_panel;
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

	////////////// DRAW SCORE /////////////////////////
	glm::vec2 screen_coords = Scene::Get2D(Position(), GetViewMatrix(), GetPerspectiveMatrix(), Window::width, Window::height);
	//Scene::sprite_renderer->DrawSprite(*info_panel, glm::vec2(screen_coords.x - 100, screen_coords.y - 400), glm::vec2(info_panel->Width(), info_panel->Height()), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	char score[5];
	strcpy_s(score, "[");
	strcat_s(score, std::to_string(num_eggs).c_str());
	strcat_s(score, "]");
	TextRenderer::RenderText(score, screen_coords.x, screen_coords.y - 400, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Player::MoveTo(float x, float y, float z)
{
    Entity::MoveTo(x, y, z);
    CalculateCameraPosition();
    CalculateCameraFront();
    ChangeState(State::WALK);
}

void Player::RotateTo(const glm::quat& newOrientation)
{
    glm::mat3 rotation = static_cast<glm::mat3>(glm::quat(newOrientation));
    Entity::RotateTo(rotation);
    CalculateCameraPosition();
    CalculateCameraFront();
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
static int tick = 0;
// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Player::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= 0.10f;
    yoffset *= 0.03f;

    // Update Front, Right and Up Vectors using the updated Eular angles

    this->toWorld = this->toWorld * glm::rotate(glm::mat4(1.0f), glm::radians(-xoffset), glm::vec3(0.0f, 1.0f, 0.0f));

    camAngle += glm::radians(yoffset);
    const static float pi2 = glm::pi<float>()/2;
    camAngle = (camAngle > pi2) ? pi2 : ((camAngle < -pi2) ? -pi2 : camAngle);

    CalculateCameraPosition();
    CalculateCameraFront();

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
    //return glm::vec3(toWorld * glm::rotate(glm::mat4(1.0f), camAngle, glm::vec3(-1.0f, 0.0f, 0.0f)) * glm::vec4(camera->Position(), 1.0f));
    return camera->Position();
}

glm::mat4 Player::GetViewMatrix() const
{
    //return camera->GetViewMatrix() * glm::rotate(glm::mat4(1.0f), camAngle, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::inverse(toWorld);
    return camera->GetViewMatrix();
}

glm::mat4 Player::GetPerspectiveMatrix() const
{
    return camera->GetPerspectiveMatrix();
}

glm::mat3 Player::GetNormalMatrix() const
{
    return this->normalMatrix;
}

void Player::ChangeState(State state)
{
    SetState(state);
    //switch (state)
    //{
    //    case State::IDLE:
    //        m_model->Reset();
    //        break;
    //    case State::WALK:
    //        m_model->PlayAnimation(m_animNames["walk"]);
    //        m_lastTime_t = Utils::CurrentTime();
    //        m_lastPos_t = Position();
    //        break;
    //    case State::JUMP:
    //        m_model->PlayAnimation(m_animNames["jump"]);
    //        break;
    //    case State::PECK:
    //        m_model->PlayAnimation(m_animNames["peck"]);
    //        break;
    //}
}

// AnimationPlayer::Listener
void Player::OnFinish()
{
    SetState(State::IDLE);
}

void Player::SetRelativeCamPosition(glm::vec3 relativePos)
{
    relativeCamPosition = relativePos;
    relativeCamPerpendicular = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), relativePos);    // Hardcoded world up
}

void Player::CalculateCameraPosition()
{
    camera->position = Position() + (glm::mat3(this->toWorld)
        * glm::mat3(glm::rotate(glm::mat4(1.0f), this->camAngle, relativeCamPerpendicular))
        * relativeCamPosition) / this->scale;     // Divide by scale
}

void Player::CalculateCameraFront()
{
    camera->front = glm::normalize(glm::mat3(this->toWorld)
                                    * glm::mat3(glm::rotate(glm::mat4(1.0f), this->camAngle, relativeCamPerpendicular))
                                    * defaultCamFront);
    camera->front = glm::normalize(camera->front);
}

void Player::Update(float deltaTime)
{
    //if (m_state == State::WALK)
    //{
    //    if (Utils::CurrentTime() - m_lastTime_t > 1.0f)
    //    {
    //        if (m_lastPos_t == Position())
    //            ChangeState(State::IDLE);
    //    }
    //}
    //m_model->Update();
}

void Player::Spawn(float x, float y, float z)
{
}
