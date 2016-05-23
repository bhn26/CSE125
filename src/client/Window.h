#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "StateManager.h"

struct GLFWwindow;

class Window
{
public:
    static int width;
    static int height;

    static bool firstMouse;
    static bool mouseCaptured;
    static GLint lastX;
    static GLint lastY;

	static CStateManager* m_pStateManager;

    static void Initialize_objects();
    static void Clean_up();
    static GLFWwindow* Create_window(int width, int height);
    static void Resize_callback(GLFWwindow* window, int width, int height);
    static void Idle_callback();
    static void Display_callback(GLFWwindow*);
    static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void Mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void Mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

