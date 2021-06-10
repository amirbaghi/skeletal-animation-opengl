#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "Scene.h"

namespace Game
{
    static Scene *scene;
    static GLFWwindow *window;

    static glm::vec2 pressed_mouse_pos;
    static glm::vec2 current_mouse_pos;
    static bool is_mouse_pressed;

    static bool inverse_kinematic;
    static int currentIteration;
    static int maxIteration;

    static void errorCallback(int error, const char *description);
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
    glm::vec2 viewport2Camera(glm::vec2 position);

    void processInput();
    void init();
    void update();
    void render();

    int main(int argc, char **argv);
}