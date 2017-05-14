#include "window.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/Objects/Chicken.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Scene.h"
#include "Player.h"
#include "Graphics/Objects/Entity.h"
#include "MenuState.h"
#include "LoadState.h"
#include "ConfigManager.h"

const char* window_title = "Egg Scramble!";

// Chicken * chicken;
#include "ClientGame.h"

// GLint shaderProgram;

int Window::s_width;
int Window::s_height;
bool Window::s_firstMouse = true;
bool Window::s_mouseCaptured = false;
GLint Window::s_lastX = s_width / 2;
GLint Window::s_lastY = s_height / 2;
int i = 0;

CStateManager* Window::m_pStateManager = new CStateManager();

void Window::Initialize_objects()
{
    // chicken = new Chicken();
}

void Window::Clean_up()
{
}

GLFWwindow* Window::Create_window(int width, int height)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        glfwTerminate();
        return nullptr;
    }

    // glEnable(GL_CULL_FACE);

    // enable highest version supported by the OS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

// Create the GLFW window

#ifdef _DEBUG
    GLFWwindow* window = glfwCreateWindow(width, height, window_title, nullptr, nullptr);
#else
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    GLFWwindow* window = glfwCreateWindow(
        mode->width, mode->height, window_title, monitor, nullptr); // for full screen mode
#endif

    // Check if the window could not be created
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return nullptr;
    }

    // Make the context of the window
    glfwMakeContextCurrent(window);

    // HWND hwnd = GetForegroundWindow(); old font stuff
    // HDC device_context = GetDC(hwnd);

    // Set swap interval to 1
    glfwSwapInterval(1);

    // Call the resize callback to make sure things get drawn immediately
    glfwGetFramebufferSize(window, &width, &height);
    Window::Resize_callback(window, width, height);

    return window;
}

void Window::Resize_callback(GLFWwindow* window, int width, int height)
{
    Window::s_width = width;
    Window::s_height = height;
    // Set the viewport size
    glViewport(0, 0, width, height);
}

void Window::Idle_callback()
{
    // Call the update function of the current object drawPtr is pointing to
    // In this instance, drawPtr is pointing to a Cube object and is therefore
    // causing the cube to rotate via its spin function.
    // chicken->Update();
    // Scene::Instance()->Update();

    DWORD dwCurrentTime = GetTickCount();
    m_pStateManager->Update(dwCurrentTime);
}

void Window::Display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object drawPtr is pointing to
    // Scene::Instance()->Draw();
    m_pStateManager->Draw();

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
        m_pStateManager->GetActiveState()->OnKeyDown(action, key);
    }
    else if (action == GLFW_RELEASE)
    {
        m_pStateManager->GetActiveState()->OnKeyUp(action, key);
    }
}

void Window::Mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (s_mouseCaptured)
    {
        if (s_firstMouse)
        {
            s_lastX = (GLuint)xpos;
            s_lastY = (GLuint)ypos;
            s_firstMouse = false;
        }

        GLfloat xoffset = (GLfloat)(xpos - s_lastX);
        GLfloat yoffset = (GLfloat)(s_lastY - ypos);
        s_lastX = (GLint)xpos;
        s_lastY = (GLint)ypos;

        // Scene::Instance()->GetPlayer()->ProcessMouseMovement(xoffset, yoffset);
        m_pStateManager->GetActiveState()->OnMouseMove(xoffset, yoffset);
    }
}

void Window::Mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    m_pStateManager->GetActiveState()->OnClick(button, action, x, y);
}

void Window::Char_callback(GLFWwindow* window, unsigned int codepoint)
{
    m_pStateManager->GetActiveState()->OnChar(codepoint);
}
