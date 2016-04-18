#include "window.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics/Objects/Cube.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera.h"


const char* window_title = "GLFW Starter Project";
Cube* cube;
Camera* camera;
//GLint shaderProgram;

int Window::width;
int Window::height;

void Window::Initialize_objects()
{
    cube = new Cube();
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    cube->shader.SetShaders("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
    //shaderProgram = LoadShaders("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
}

void Window::Clean_up()
{
    delete(cube);
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
    cube->Update();
}

void Window::Display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader of programID
    //glUseProgram(shaderProgram);

    // Render the object drawPtr is pointing to
    cube->Draw(camera->GetViewMatrix());

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
                camera->ProcessKeyboard(Camera_Movement::FORWARD, 1);
                break;
            case GLFW_KEY_A:
                camera->ProcessKeyboard(Camera_Movement::LEFT, 1);
                break;
            case GLFW_KEY_S:
                camera->ProcessKeyboard(Camera_Movement::BACKWARD, 1);
                break;
            case GLFW_KEY_D:
                camera->ProcessKeyboard(Camera_Movement::RIGHT, 1);
                break;
            case GLFW_KEY_SPACE:
                camera->ProcessKeyboard(Camera_Movement::UP, 1);
                break;
            case GLFW_KEY_Z:
                camera->ProcessKeyboard(Camera_Movement::DOWN, 1);
                break;
            default:
                break;
        }
    }
}

bool firstMouse = true;
GLuint lastX = Window::width / 2;
GLuint lastY = Window::height / 2;

void Window::Mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera->ProcessMouseMovement(xoffset, yoffset);
}

