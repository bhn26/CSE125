#include "Player.h"

#include "Graphics/Camera.h"
#include "Graphics/Objects/Chicken.h"
#include "Graphics/Shader.h"
#include "Graphics/Scene.h"
#include "Graphics/PointLight.h"

Player::Player() : toWorld(glm::mat4(1.0f))
{
    model = std::make_unique<Model>("assets/chickens/objects/chicken.obj");
    camera = std::make_unique<Camera>(glm::vec3(2.0f, 1.0f, -3.0f));
}

Player::~Player()
{
}

void Player::Draw() const
{
    shader->Use();
    glm::mat4 projection = glm::perspective(camera->Zoom(), (float)Window::width/(float)Window::height, 0.1f, 1000.0f);

    GLint viewLoc = shader->GetUniform("view");
    GLint modelLocation = shader->GetUniform("model");
    GLint normalMatrixLoc = shader->GetUniform("normalMatrix");
    GLint projectionLocation = shader->GetUniform("projection");
    GLint lightColorLoc = shader->GetUniform("lightColor");
    GLint lightPosLoc = shader->GetUniform("lightPos");
    GLint viewPosLoc = shader->GetUniform("viewPos");

    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(toWorld * camera->GetViewMatrix()));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(normalMatrixLoc, 1, false, glm::value_ptr(GetNormalMatrix()));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));

    glUniform3fv(lightColorLoc, 1, glm::value_ptr(Scene::pLight->color));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(Scene::pLight->position));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(CameraPosition()));

    model->Draw(shader.get());
}

// Process movement
void Player::ProcessKeyboard(DIRECTION direction, GLfloat deltaTime)
{
    GLfloat velocity = camera->Speed() * deltaTime;
    if (direction == D_FORWARD)
        this->toWorld = glm::translate(glm::mat4(1.0f), camera->Front() * velocity) * this->toWorld;
    if (direction == D_BACKWARD)
        this->toWorld = glm::translate(glm::mat4(1.0f), camera->Front() * -velocity) * this->toWorld;
    if (direction == D_LEFT)
        this->toWorld = glm::translate(glm::mat4(1.0f), camera->Right() * velocity) * this->toWorld;
    if (direction == D_RIGHT)
        this->toWorld = glm::translate(glm::mat4(1.0f), camera->Right() * -velocity) * this->toWorld;
    if (direction == D_UP)
        this->toWorld = glm::translate(glm::mat4(1.0f), camera->Up() * velocity) * this->toWorld;
    if (direction == D_DOWN)
        this->toWorld = glm::translate(glm::mat4(1.0f), camera->Up() * -velocity) * this->toWorld;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Player::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    camera->ProcessMouseMovement(xoffset, yoffset, constrainPitch);
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Player::ProcessMouseScroll(GLfloat yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

glm::vec3 Player::CameraPosition() const
{
    return glm::vec3(toWorld * glm::vec4(camera->Position(), 1.0f));
}

glm::mat4 Player::GetViewMatrix() const
{
    return toWorld * camera->GetViewMatrix();
}

glm::mat3 Player::GetNormalMatrix() const
{
    return glm::mat3(glm::transpose(glm::inverse(toWorld)));
}