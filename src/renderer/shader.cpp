#include "playgroundpch.h"
#include "shader.h"

ShaderSource Shader::ParseShader(const std::string& filepath)
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
GLuint Shader::CompileShader(unsigned int type, const std::string& source)
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


GLuint Shader::CreateShader(const ShaderSource& shaderSource)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, shaderSource.VertexSource);
    unsigned int gs = 0;
    if (shaderSource.GeometrySource.empty() == false)
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