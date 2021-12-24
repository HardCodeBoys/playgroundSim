#pragma once

#include <GLFW/glfw3.h>

#include <iostream>
#include "scene.h"

constexpr float entitiesSpeed = 0.5f;

class InputManager {
private:
    InputManager() {}
public:
    inline static bool IS_RUNNING;
    inline static bool SHOW_GUI;
    // TODO: Inline for some reason ??? figure this out
    inline static std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
            std::cout << "pressed mouse button 1" << std::endl;
            //scene->CreateCube(glm::vec3(0, 1, 1), 1, 1);
            double xPos = 0, yPos = 0;
            glfwGetCursorPos(window, &xPos, &yPos);
            std::cout << "at position (x = " << xPos << ", y = " << yPos << ")" << std::endl;
            scene->SelectEntity(xPos, yPos);
        }
        if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
            std::cout << "pressed mouse button 2" << std::endl;
            scene->DeleteEntity(glm::vec3(0, 1, 1));
        }
    }

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwDestroyWindow(window);
        }
        if (key == GLFW_KEY_X && action == GLFW_PRESS) {
            scene->PrintAllEntities();
        }
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            SHOW_GUI = !SHOW_GUI;
        }
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            scene->MoveEntities(glm::vec3(0, 1 * entitiesSpeed, 0));
        }
        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            scene->MoveEntities(glm::vec3(0, -1 * entitiesSpeed, 0));
        }
        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            scene->MoveEntities(glm::vec3(0, 0, 1 * entitiesSpeed));
        }
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            scene->MoveEntities(glm::vec3(0, 0, -1 * entitiesSpeed));
        }
	}
    static void ProcessKeyEvents(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            scene->renderer->camera.MoveForward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            scene->renderer->camera.MoveBackwards(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            scene->renderer->camera.StrafeLeft(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            scene->renderer->camera.StrafeRight(deltaTime);
        }

    }
};