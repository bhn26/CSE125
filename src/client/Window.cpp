#include "window.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Graphics/Objects/Chicken.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Scene.h"
#include "Player.h"

const char* window_title = "Egg Scramble!";

//Chicken * chicken;
#include "ClientGame.h"

//GLint shaderProgram;

int Window::width;
int Window::height;
bool Window::firstMouse = true;
bool Window::mouseCaptured = false;
GLint Window::lastX = width / 2;
GLint Window::lastY = height / 2;

void Window::Initialize_objects()
{
    //chicken = new Chicken();
}

void Window::Clean_up()
{
    //delete(chicken);
    //glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::Create_window(int width, int height)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        glfwTerminate();
        return NULL;
    }

    // enable highest version supported by the OS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    
    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

    // Check if the window could not be created
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window
    glfwMakeContextCurrent(window);

    // Set swap interval to 1
    glfwSwapInterval(1);

    // Call the resize callback to make sure things get drawn immediately
    glfwGetFramebufferSize(window, &width, &height);
    Window::Resize_callback(window, width, height);

    return window;
}

void Window::Resize_callback(GLFWwindow* window, int width, int height)
{
    Window::width = width;
    Window::height = height;
    // Set the viewport size
    glViewport(0, 0, width, height);
}

void Window::Idle_callback()
{
    // Call the update function of the current object drawPtr is pointing to
    // In this instance, drawPtr is pointing to a Cube object and is therefore
    // causing the cube to rotate via its spin function.
    //chicken->Update();
    Scene::Instance()->Update();
}

void Window::Display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader of programID
    //glUseProgram(shaderProgram);

    // Render the object drawPtr is pointing to
    //chicken->Draw(camera);
    Scene::Instance()->Draw();

    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);
}

void Window::Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            // Check if escape was pressed
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_W:
                //Scene::player->ProcessKeyboard(DIRECTION::D_FORWARD, 1);
#ifdef _WIN32
                ClientGame::instance()->sendMovePacket(MOVE_FORWARD);
#endif
                break;
            case GLFW_KEY_A:
                //Scene::player->ProcessKeyboard(DIRECTION::D_LEFT, 1);
#ifdef _WIN32
                ClientGame::instance()->sendMovePacket(MOVE_LEFT);
#endif
                break;
            case GLFW_KEY_S:
                //Scene::player->ProcessKeyboard(DIRECTION::D_BACKWARD, 1);
#ifdef _WIN32
                ClientGame::instance()->sendMovePacket(MOVE_BACKWARD);
#endif
                break;
            case GLFW_KEY_D:
                //Scene::player->ProcessKeyboard(DIRECTION::D_RIGHT, 1);
                ClientGame::instance()->sendMovePacket(MOVE_RIGHT);
                break;
            case GLFW_KEY_SPACE: // jump?
                Scene::Instance()->GetPlayer()->ProcessKeyboard(DIRECTION::D_UP, 1);
                break;
            case GLFW_KEY_Z:
                Scene::Instance()->GetPlayer()->ProcessKeyboard(DIRECTION::D_DOWN, 1);
                break;
            default:
                break;
        }
    }
}


void Window::Mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseCaptured)
    {
        if (firstMouse)
        {
            lastX = (GLuint)xpos;
            lastY = (GLuint)ypos;
            firstMouse = false;
        }

        GLfloat xoffset = (GLfloat)(xpos - lastX);
        GLfloat yoffset = (GLfloat)(lastY - ypos);
        lastX = (GLint)xpos;
        lastY = (GLint)ypos;
        Scene::Instance()->GetPlayer()->ProcessMouseMovement(xoffset, yoffset);
    }
}

void Window::Mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && !mouseCaptured)
    {
        mouseCaptured = true;
        firstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, Window::width/2, Window::height/2);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && mouseCaptured)
    {
        mouseCaptured = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
