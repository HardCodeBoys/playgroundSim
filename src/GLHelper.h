#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>

#include <vector>

constexpr unsigned int SCREEN_HEIGHT = 640;
constexpr unsigned int SCREEN_WIDTH = 960;


// debugging macros, stolen from Cherno
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearErorrs();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))\

static void GLClearErorrs()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
#if _DEBUG
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << " " << line << std::endl;
        return false;
    }
#endif
    return true;
}
