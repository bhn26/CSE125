#include "Player.h"

#include "Graphics/Camera.h"
#include "Graphics/Objects/Chicken.h"
#include "Graphics/Shader.h"
#include "Graphics/Scene.h"
#include "Graphics/PointLight.h"

Player::Player() : toWorld(glm::mat4(1.0f)), camAngle(0.0f)
{
    model = std::unique_ptr<Model>(new Model("assets/chickens/objects/chicken.obj"));
    camera = std::unique_ptr<Camera>(new Camera(glm::vec3(-1.5f, 4.5f, -7.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, -15.0f));
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

    //glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::camera->GetViewMatrix()));
    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(this->GetViewMatrix()));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix3fv(normalMatrixLoc, 1, false, glm::value_ptr(GetNormalMatrix()));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));

    glUniform3fv(lightColorLoc, 1, glm::value_ptr(Scene::pLight->color));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(Scene::pLight->position));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(this->CameraPosition()));
    //glUniform3fv(viewPosLoc, 1, glm::value_ptr(Scene::camera->Position()));

    model->Draw(shader.get());
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

glm::mat3 Player::GetNormalMatrix() const
{
    return glm::mat3(glm::transpose(glm::inverse(toWorld)));
}