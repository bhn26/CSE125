#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "StateManager.h"

struct GLFWwindow;

class Window
{
public:
    static int s_width;
    static int s_height;

    static bool s_firstMouse;
    static bool s_mouseCaptured;
    static GLint s_lastX;
    static GLint s_lastY;

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
    static void Char_callback(GLFWwindow* window, unsigned int codepoint);
};
