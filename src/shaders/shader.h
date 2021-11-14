#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

struct ShaderSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    GLuint ID;
public:
    Shader() {}
    Shader(GLuint _ID) : ID(_ID) {}
    Shader(const Shader& s) : ID(s.ID) { std::cout << "shader cpy ctor" << std::endl; }
    Shader(const Shader&& s) : ID(s.get_id()) {}
    Shader(const std::string& path) : ID(0) {
        ShaderSource s = parse_shader(path);
        ID = create_shader(s.VertexSource, s.FragmentSource);
    }
    GLuint get_id() const {
        return ID;
    }
    void use() const {
        glUseProgram(ID);
    }
    void set_vec3(const std::string& name, const glm::vec3& v) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
    }
    void set_vec3(const std::string& name, float x, float y, float z) const {
        if (glGetUniformLocation(ID, name.c_str()) < 0)
            std::cout << "SHADER ERROR, WRONG UNIFORM LOCATION";
        std::cout << "id :" << glGetUniformLocation(ID, name.c_str());
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void set_mat4(const std::string& name, const glm::mat4& m) const {
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



private:
    ShaderSource parse_shader(const std::string& filepath)
    {
        std::ifstream file(filepath);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::string line;

        std::stringstream ss[2];

        ShaderType type = ShaderType::NONE;

        while (getline(file, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
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
        return { ss[0].str(), ss[1].str() };
    }
    GLuint compile_shader(unsigned int type, const std::string& source)
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
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }
        return id;
    }


    GLuint create_shader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);
        return program;
    }
};