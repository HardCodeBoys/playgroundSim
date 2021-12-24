/* Playground simulation made with OpenGl
* DEPENDENCIES:
* GLFW, 
* GLEW,
* GLM,
* ImGui
* 
* 
* TODO: 
*       Entity's position is duplicated in Entity and Model and Collider and its Model ???
*       figure out which functions should be const
*       create a crosshair
*       figure out the shared/unique ptr madness
*       figure out the mersenne twister prng
* 
*       RENDERING
*       ??? create a model loader/saver
*       definitely play some more with meshes
*       materials/light doesnt work really well
*       the position of Light is hardcoded and currently only one working light
*       add rotating camera like in an fps/3d game
* 
*       PHYSICS
*       colliders, rigid bodies
*       
*       WORLD
*       model the map out of triangles
* 
*       GUI
*       pause menu with escape
* 
* made by HardCodeBoys
* */

#include "GLHelper.h"


#include "playground.h"
#include "shader.h"

#include "scene.h"
#include "renderer.h"
#include "cube.h"
#include "input_manager.h"
#include "gui.h"

int main()
{
    GLFWwindow* window;
    // Initialize the library 
    if (!glfwInit())
    {
        std::cout << "Failed to initialize Glfw";
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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

    

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Transparency functions
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_CULL_FACE);
    // Draw wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Accept fragment if it's closer to the camera than the former one
    glDepthFunc(GL_LESS);

    meth::Random::Init(50);
    LogNS::Init();
    Log::Init();

    Shader basicShader("res/shaders/basic.shader");
    Shader lightShader("res/shaders/light_shader.shader");
    Shader wireframeShader("res/shaders/wireframe_shader.shader");

    glm::vec3 lightPosition(1, 1, 0);

    basicShader.Use();
    basicShader.SetVec3("lightColor", 1, 1, 1);
    basicShader.SetVec3("lightPos", lightPosition);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    InputManager::scene = scene;

    std::cout << "creating scene" << std::endl;

    scene->renderer->AddShader(wireframeShader);
    scene->wireframeShader = wireframeShader.ID;

    scene->CreateCube(glm::vec3(0, 0, 0), 1, basicShader);
    scene->CreateSphere(glm::vec3(0, 1, 1), 1, basicShader);
    scene->CreateCube(glm::vec3(0, 1, -1), 1, basicShader);
    //scene->CreatePlane(glm::vec3(0, -3, 0), 10, basicShader);
    scene->CreateLight(lightPosition, lightShader);
    scene->CreateTerrain(glm::vec3(0, -10, 0), 50, wireframeShader);
    //scene->CreateTerrain(glm::vec3(20, -10, 0), 10, wireframeShader);
    scene->PrintAllEntities();
    auto lam = [&](int a, int b) {return a < b; };

    float deltaTime = 0.f;
    double currentTime = glfwGetTime();
    double lastTime = 0;

    glfwSetMouseButtonCallback(window, InputManager::mouse_button_callback);
    glfwSetKeyCallback(window, InputManager::key_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    int movementCounter = 0;
    std::cout << glGetString(GL_VERSION) << std::endl;
    glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    GUI::Init(window);

    InputManager::IS_RUNNING = true;
    InputManager::SHOW_GUI = false;


    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        deltaTime = (float)(currentTime - lastTime);
        lastTime = glfwGetTime();

        glClearColor(0, 0.12f, 0.6f, 1.0f);
        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        scene->renderer->camera.RotateCamera(deltaTime, (float)xpos, (float)ypos);

        // vibrate the scene with time
        // scene->MoveEntitys(glm::vec3(0, std::sin(currentTime) * deltaTime, 0));

        if(InputManager::IS_RUNNING) scene->RenderScene();
        if(InputManager::SHOW_GUI) GUI::DrawTables();

        // Swap front and back buffers 
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Poll for and process events 
        InputManager::ProcessKeyEvents(window, deltaTime);
        
    }
    // TODO: deleting shaders, should probably clean up more of everything
    glDeleteProgram(basicShader.ID);
    glDeleteProgram(lightShader.ID);
    glDeleteProgram(wireframeShader.ID);

    // Deletes all ImGUI instances
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}