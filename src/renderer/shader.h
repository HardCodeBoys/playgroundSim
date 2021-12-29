#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils/log.h"

#include <iostream>
#include <fstream>
#include <sstream>

struct ShaderSource
{
    std::string VertexSource;
    std::string GeometrySource;
    std::string FragmentSource;
};

class Shader {
public:
    GLuint ID;
private:
public:
    Shader() {}
    Shader(GLuint _ID) : ID(_ID) {}
    Shader(const Shader& s) { std::cout << "shader cpy ctor" << std::endl; }
    Shader(Shader&& s) noexcept { std::cout << "shader move ctor" << std::endl; }
    Shader(const std::string& path) : 
        ID(0) {
        ShaderSource s = ParseShader(path);
        ID = CreateShader(s);
    }
    Shader& operator=(Shader other) {
        this->ID = other.ID;
    }
    void Use() const {
        glUseProgram(ID);
    }
    void SetVec3(const std::string& name, const glm::vec3& v) const {
        if (glGetUniformLocation(ID, name.c_str()) < 0) {
            PL_ERROR(("SHADER ERROR, WRONG UNIFORM LOCATION"));
            return;
        }
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
    }
    void SetVec3(const std::string& name, float x, float y, float z) const {
        if (glGetUniformLocation(ID, name.c_str()) < 0) {
            PL_ERROR("SHADER ERROR, WRONG UNIFORM LOCATION");
            return;
        }
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void SetMat4(const std::string& name, const glm::mat4& m) const {
        if (glGetUniformLocation(ID, name.c_str()) < 0) {
            PL_ERROR("SHADER ERROR, WRONG UNIFORM LOCATION");
            return;
        }
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &m[0][0]);
    }
    void print_vec3(const std::string& name) const {
        float data[3];
        std::cout << "printvec3" << std::endl;
        std::cout << glGetUniformLocation(ID, name.c_str()) << std::endl;
        glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), data);
        std::cout << data[0] << std::endl;
        std::cout << data[1] << std::endl;
        std::cout << data[2] << std::endl;
    }                                


    explicit operator unsigned int() const { return ID; }


private:
    ShaderSource ParseShader(const std::string& filepath);
    GLuint CompileShader(unsigned int type, const std::string& source);
    GLuint CreateShader(const ShaderSource& shaderSource);
};