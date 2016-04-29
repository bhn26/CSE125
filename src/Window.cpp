#include "window.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/Objects/Chicken.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"
#include "Graphics/Scene.h"

const char* window_title = "Egg Scramble!";

//Chicken * chicken;
//GLint shaderProgram;

int Window::width;
int Window::height;
bool Window::firstMouse = true;
GLuint Window::lastX = width / 2;
GLuint Window::lastY = height / 2;

float shiftVal = 0.5;
float scaleVal = 0.9;
float orbitAngle = 10;
bool rotX = false;
bool rotY = true;
bool rotZ = false;

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
    Scene::Update(rotX, rotY, rotZ);
}

void Window::Display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader of programID
    //glUseProgram(shaderProgram);

    // Render the object drawPtr is pointing to
    //chicken->Draw(camera);
    Scene::Draw();

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
            case GLFW_KEY_F1:
                rotX = !rotX;
                break;
            case GLFW_KEY_F2:
                rotY = !rotY;
                break;
            case GLFW_KEY_F3:
                rotZ = !rotZ;
                break;
            case GLFW_KEY_W:
                Scene::camera->ProcessKeyboard(Camera_Movement::FORWARD, 1);
                break;
            case GLFW_KEY_A:
                Scene::camera->ProcessKeyboard(Camera_Movement::LEFT, 1);
                break;
            case GLFW_KEY_S:
                Scene::camera->ProcessKeyboard(Camera_Movement::BACKWARD, 1);
                break;
            case GLFW_KEY_D:
                Scene::camera->ProcessKeyboard(Camera_Movement::RIGHT, 1);
                break;
            case GLFW_KEY_Z:
                Scene::camera->ProcessKeyboard(Camera_Movement::DOWN, 1);
                break;
            case GLFW_KEY_SPACE:
                Scene::camera->ProcessKeyboard(Camera_Movement::UP, 1);
                break;
            //case GLFW_KEY_X:
            //    if (mods == GLFW_MOD_SHIFT) Scene::chicken->ShiftX(0.5);
            //    else Scene::chicken->ShiftX(-0.5);
            //    break;
            //case GLFW_KEY_Y:
            //    if (mods == GLFW_MOD_SHIFT) Scene::chicken->ShiftY(shiftVal);
            //    else Scene::chicken->ShiftY(-shiftVal);
            //    break;
            //case GLFW_KEY_Z:
            //    if (mods == GLFW_MOD_SHIFT) Scene::chicken->ShiftZ(shiftVal);
            //    else Scene::chicken->ShiftZ(-shiftVal);
            //    break;
            //case GLFW_KEY_S:
            //    if (mods == GLFW_MOD_SHIFT) Scene::chicken->Scale(1/scaleVal);
            //    else Scene::chicken->Scale(scaleVal);
            //    break;
            //case GLFW_KEY_F:
            //    Scene::camera->ProcessKeyboard(Camera_Movement::FORWARD, 1);
            //    break;
            //case GLFW_KEY_RIGHT:
            //    Scene::camera->ProcessKeyboard(Camera_Movement::RIGHT, 1);
            //    break;
            //case GLFW_KEY_B:
            //    Scene::camera->ProcessKeyboard(Camera_Movement::BACKWARD, 1);
            //    break;
            //case GLFW_KEY_LEFT:
            //    Scene::camera->ProcessKeyboard(Camera_Movement::LEFT, 1);
            //    break;
            //case GLFW_KEY_DOWN:
            //    Scene::camera->ProcessKeyboard(Camera_Movement::DOWN, 1);
            //    break;
            //case GLFW_KEY_UP:
            //    Scene::camera->ProcessKeyboard(Camera_Movement::UP, 1);
            //    break;
            default:
                break;
        }
    }
}


void Window::Mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == 1)
    {
        if (firstMouse)
        {
            lastX = (GLuint) xpos;
            lastY = (GLuint) ypos;
            firstMouse = false;
        }

        GLfloat xoffset = (GLfloat) (xpos - lastX);
        GLfloat yoffset = (GLfloat) (lastY - ypos);
        lastX = (GLuint)xpos;
        lastY = (GLuint)ypos;
        Scene::camera->ProcessMouseMovement(xoffset, yoffset);
    }
    else
    {
        firstMouse = true;
    }
}

