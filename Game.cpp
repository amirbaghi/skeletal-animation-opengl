#include "./Headers/Game.h"

#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <cmath>

void Game::errorCallback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void Game::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_6 && action == GLFW_PRESS)
    {
        scene->selectBone(key - GLFW_KEY_1);
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        inverse_kinematic = !inverse_kinematic;
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if (scene->isAnimationPlaying())
        {
            scene->stopAnimation();
        }
        else
        {
            scene->playAnimation();
        }
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        should_clear_animation = true;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        should_add_keyframe = true;
    }
}

void Game::mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        is_mouse_pressed = true;

        pressed_mouse_pos = glm::vec2(x, y);

        if (inverse_kinematic)
        {
            currentIteration = 0;
        }
    }
    else
    {
        is_mouse_pressed = false;
    }
}

void Game::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    current_mouse_pos = glm::vec2(xpos, ypos);
}

glm::vec2 Game::viewport2Camera(glm::vec2 position)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    float ratio = (float)width / height;

    return glm::vec2(position.x * (150 * ratio / width) - 75 * ratio, -position.y * 150 / height + 75);
}

void Game::init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    inverse_kinematic = false;
    currentIteration = 0;
    maxIteration = 100;

    should_play_animation = false;
    should_clear_animation = false;
    should_add_keyframe = false;

    frameRate = 40;

    scene = new Scene();

    scene->addBone("", "Bone 1", 15, 0);
    scene->addBone("Bone 1", "Bone 2", 15, 0);
    scene->addBone("Bone 2", "Bone 3", 15, 0);
    scene->addBone("Bone 3", "Bone 4", 15, 0);
    scene->addBone("Bone 4", "Bone 5", 15, 0);
    scene->addBone("Bone 5", "Bone 6", 15, 0);

    scene->createNewAnimation(frameRate * 5);

    scene->init();

    float segments = 100.0;

    std::vector<Vertex> skin;
    int boneCount = scene->getCount();
    float x = 0.0;
    for (int i = 0; i < boneCount; i++)
    {
        int previousBoneIndex = fmax(0, i - 1);
        int nextBoneIndex = fmin(boneCount - 1, i + 1);

        Bone *previousBone = scene->getBone(previousBoneIndex);
        Bone *bone = scene->getBone(i);
        Bone *nextBone = scene->getBone(nextBoneIndex);

        float step = bone->getLength() / segments;
        for (int j = 0; j < segments; j++)
        {
            x += step;

            for (auto y : {5.f, -5.f})
            {
                if (j < segments / 2)
                {
                    skin.push_back(Vertex{x, y, 0, 0, previousBoneIndex, i, (0.5 - j / segments), (0.5 + j / segments)});
                }
                else
                {
                    skin.push_back(Vertex{x, y, 0, 0, i, nextBoneIndex, (1.0 - (j - segments / 2.0) / segments), ((j - segments / 2.0) / segments)});
                }
            }
        }
    }

    scene->setSkin(skin);
}

void Game::processInput()
{
    if (is_mouse_pressed && !inverse_kinematic)
    {
        auto diff = current_mouse_pos - pressed_mouse_pos;
        float angle = glm::atan(-diff.y, diff.x);
        scene->rotateSelectedBone(angle);
    }
    else if (inverse_kinematic)
    {
        if (currentIteration < maxIteration)
        {
            auto pos = viewport2Camera(pressed_mouse_pos);
            scene->inverseKinematic(pos);
            currentIteration++;
        }
    }
    if (should_add_keyframe && !should_play_animation)
    {
        scene->addKeyframe();
        should_add_keyframe = false;
    }
    if (should_clear_animation)
    {
        scene->createNewAnimation(frameRate * 5);
        should_clear_animation = false;
    }

    glfwPollEvents();
}

void Game::update()
{
    double ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    ratio = (float)width / height;

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-ratio * 75.f, ratio * 75.f, -75.f, 75.f);

    scene->update();
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    auto pos = viewport2Camera(pressed_mouse_pos);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(-5 + pos.x, pos.y);
    glVertex2f(5 + pos.x, pos.y);
    glVertex2f(pos.x, -5 + pos.y);
    glVertex2f(pos.x, 5 + pos.y);
    glEnd();

    scene->render();

    glfwSwapBuffers(window);
}

int Game::main(int argc, char **argv)
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 8);

    window = glfwCreateWindow(640, 480, "Skeletal Animation", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    glfwMakeContextCurrent(window);
    glewInit();

    init();

    double prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double current_time = glfwGetTime();
        double delta = current_time - prev_time;

        if (delta >= 1.0 / 40)
        {
            processInput();
            update();
            render();

            prev_time = current_time;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    glfwDestroyWindow(window);

    return 0;
}