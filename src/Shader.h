
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"
#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader
{
    public:
        Shader(const std::string &filepath);
        void Bind();
        void Unbind();

        void SetUniform1i(const std::string &name, int value);
        void SetUniform1f(const std::string &name, float value);
        void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

        void SetUniformMatrix4fv(const std::string &name, glm::mat4 &matrix);

    private:
        ShaderProgramSource ParseShader(const std::string &filepath);
        unsigned int CompileShader(unsigned int type, const std::string &source);
        unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
        int Shader::GetUniformLocation(const std::string &name);
    private:
        std::string m_FilePath;
        unsigned int m_RendererID;
        std::unordered_map<std::string, int> m_UniformLocationCache;

};

Shader::Shader(const std::string &filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    Bind();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return(0);
    }

    return(id);
}


ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{

    std::ifstream stream(filepath);

    enum ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    ShaderProgramSource result;
    result.VertexSource = ss[0].str();
    result.FragmentSource = ss[1].str();
    return(result);
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return(program);
}

void Shader::Bind()
{
    glUseProgram(m_RendererID);
}

int Shader::GetUniformLocation(const std::string &name)
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return(m_UniformLocationCache[name]);
    }
    return(glGetUniformLocation(m_RendererID, name.c_str()));
}


void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetUniform1f(const std::string &name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformMatrix4fv(const std::string &name, glm::mat4 &matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}
