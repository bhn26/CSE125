#ifndef _WINDOW_H_
#define _WINDOW_H_

class Camera;
extern Camera* camera;

struct GLFWwindow;

class Window
{
public:
    static int width;
    static int height;

    static void Initialize_objects();
    static void Clean_up();
    static GLFWwindow* Create_window(int width, int height);
    static void Resize_callback(GLFWwindow* window, int width, int height);
    static void Idle_callback();
    static void Display_callback(GLFWwindow*);
    static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void Mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

#endif
