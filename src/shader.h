#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
            //std::cout << "SHADER ERROR, WRONG UNIFORM LOCATION FOR " << name << std::endl;
            return;
        }
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
    }
    void SetVec3(const std::string& name, float x, float y, float z) const {
        if (glGetUniformLocation(ID, name.c_str()) < 0) {
            //std::cout << "SHADER ERROR, WRONG UNIFORM LOCATION FOR " << name << std::endl;
            return;
        }
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void SetMat4(const std::string& name, const glm::mat4& m) const {
        if (glGetUniformLocation(ID, name.c_str()) < 0) {
            //std::cout << "SHADER ERROR, WRONG UNIFORM LOCATION FOR " << name << std::endl;
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
    ShaderSource ParseShader(const std::string& filepath)
    {
        std::ifstream file(filepath);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, GEOMETRY = 1, FRAGMENT = 2
        };

        std::string line;

        std::stringstream ss[3];

        ShaderType type = ShaderType::NONE;

        while (getline(file, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("geometry") != std::string::npos)
                {
                    type = ShaderType::GEOMETRY;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            else
            {
                ss[(int)type] << line << '\n';
            }
        }
        return { ss[0].str(), ss[1].str(), ss[2].str() };
    }
    GLuint CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "SHADER COMPILATION ERROR: " << message << std::endl;
            glDeleteShader(id);
            return 0;
        }
        return id;
    }


    GLuint CreateShader(const ShaderSource& shaderSource)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, shaderSource.VertexSource);
        unsigned int gs = 0;
        if(shaderSource.GeometrySource.empty() == false)
             gs = CompileShader(GL_GEOMETRY_SHADER, shaderSource.GeometrySource);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, shaderSource.FragmentSource);

        glAttachShader(program, vs);
        if (shaderSource.GeometrySource.empty() == false)
            glAttachShader(program, gs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        if (shaderSource.GeometrySource.empty() == false)
            glDeleteShader(gs);
        glDeleteShader(fs);
        return program;
    }
};