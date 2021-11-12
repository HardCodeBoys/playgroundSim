/* Playground simulation made with openGl
* DEPENDENCIES:
* GLFW, GLEW,
* GLM,
* 
* 
* TODO: 
* 
*       the highest class will be "Scene" with the information about all of the objects
*       1) create a shader class
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

#include "shaders/shader.h"

#include "renderer.h"
#include "mesh/cube.h"
#include "meth.h"


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

    GLfloat colorsData[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    Shader basicShader("res/shaders/basic.shader");
    Shader lightShader("res/shaders/light_shader.shader");

    GLuint lightColor = glGetUniformLocation(basicShader.get_id(), "lightColor");
    basicShader.use();
    glUniform3f(lightColor, 1.f, 1.f, 1.f);
    GLuint lightPos = glGetUniformLocation(lightShader.get_id(), "lightColor");
    glUniform3f(lightPos, 1, 1, 1);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it's closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorsData), colorsData, GL_STATIC_DRAW);

    Renderer renderer;
    renderer.create_cube(glm::vec3(0, 0, 0), 1, basicShader.get_id());
    renderer.create_cube(glm::vec3(5, 2, -5), 1, basicShader.get_id());
    renderer.create_cube(glm::vec3(1, 2, -5), 1, basicShader.get_id());

    renderer.create_light(glm::vec3(1, 1, 1), lightShader.get_id());



    renderer.set_color_buffer(colorbuffer);
    
    
    float deltaTime = 0.f;
    double currentTime = glfwGetTime();
    double lastTime = 0;

    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;


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
            renderer.get_camera().rotate_camera(deltaTime, xpos, ypos);
        //}


        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw_all_cubes();
        renderer.draw_all_lights();


        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            renderer.get_camera().move_forward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            renderer.get_camera().move_backwards(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            renderer.get_camera().strafe_left(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            renderer.get_camera().strafe_right(deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            std::cout << "UP";
            renderer.move_cubes(glm::vec3(0, 1, 0));
        }

        glfwSetKeyCallback(window, key_callback);
    }
    glDeleteProgram(basicShader.get_id());
    glDeleteProgram(lightShader.get_id());


    glfwTerminate();
    return 0;
}