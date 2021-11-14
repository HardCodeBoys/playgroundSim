#pragma once

#include <GLFW/glfw3.h>

#include <iostream>

class InputManager {
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        {
            std::cout << "pressed right" << std::endl;
            //Renderer::move_cubes(glm::vec3(1, 0, 0));
        }
	}
};