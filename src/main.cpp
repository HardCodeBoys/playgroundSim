/* Playground simulation made with openGl
* DEPENDENCIES:
* GLFW, GLEW,
* GLM,
* 
* 
* TODO: 
* 
*       
*       create a system for handling input
*
* 
* 
* 
* made by HardCodeBoys
* */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GLHelper.h"

#include "shader.h"

#include "Scene.h"

#include "renderer.h"
#include "cube.h"
#include "input_manager.h"
#include "meth.h"

// here i could just send the pointer to the Scene, where it will handle the input

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        std::cout << "pressed right" << std::endl;
        //Renderer::move_cubes(glm::vec3(1, 0, 0));
    }
}
int main(void)
{
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
    {
        std::cout << "Failed to initialize Glfw";
        return -1;

    }

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PLAYGROUND", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to open a GLFW window";
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "error with initializing GLEW" << std::endl;

    Shader basicShader("res/shaders/basic.shader");
    Shader lightShader("res/shaders/light_shader.shader");

    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it's closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glm::vec3 lightPosition(1, 1, 0);

    basicShader.use();
    basicShader.set_vec3("lightColor", 1, 1, 1);
    basicShader.set_vec3("lightPos", lightPosition);


    Scene scene;
    std::cout << "creating scene" << std::endl;
    scene.create_cube(glm::vec3(0, 0, 0), 1, basicShader);
    //scene.create_cube(glm::vec3(0, 1, 1), 1, basicShader);
    //scene.create_cube(glm::vec3(0, 1, -1), 1, basicShader);
    scene.create_plane(glm::vec3(0, -1, 0), 10, basicShader);

    scene.create_light(lightPosition, lightShader);

    scene.print_all_objects();

    float deltaTime = 0.f;
    double currentTime = glfwGetTime();
    double lastTime = 0;

    glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        deltaTime = (float)(currentTime - lastTime);
        lastTime = glfwGetTime();            
        
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            scene.get_renderer()->get_camera().rotate_camera(deltaTime, xpos, ypos);
        //}

        glClearColor(0, 0.12f, 0.6f, 1.0f);
        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        scene.render_scene();

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            scene.get_renderer()->get_camera().move_forward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            scene.get_renderer()->get_camera().move_backwards(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            scene.get_renderer()->get_camera().strafe_left(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            scene.get_renderer()->get_camera().strafe_right(deltaTime);
        }

        float objectsSpeed = 0.01f;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            scene.move_objects(glm::vec3(0, 1 * objectsSpeed, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            scene.move_objects(glm::vec3(0, - 1 * objectsSpeed, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            scene.move_objects(glm::vec3(0, 0, 1 * objectsSpeed));
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            scene.move_objects(glm::vec3(0, 0, -1 * objectsSpeed));
        }
        glfwSetKeyCallback(window, InputManager::key_callback);
    }
    glDeleteProgram(basicShader.get_id());
    glDeleteProgram(lightShader.get_id());


    glfwTerminate();
    return 0;
}