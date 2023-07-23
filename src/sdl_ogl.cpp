#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
typedef unsigned int u32;

#define global static


global bool is_running = false;
global int Width = 640;
global int Height = 480;

SDL_Window *Window;
SDL_GLContext Context;

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

ShaderProgramSource ParseShader(const std::string &filepath)
{
    ShaderProgramSource result;

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

    result.VertexSource = ss[0].str();
    result.FragmentSource = ss[1].str();

    return(result);
}

u32 CompileShader(u32 type, const std::string &source)
{  
    u32 id = glCreateShader(type);
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


u32 CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    
    u32 program = glCreateProgram();
    u32 vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    u32 fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return(program);
}




void GetOpenGLInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl; 
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Lang: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

int main(int ArgC, char **Args)
{
    printf("start\n");
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        is_running = true;
        printf("running");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    Window = SDL_CreateWindow("OGL Window", 0, 0, Width, Height, SDL_WINDOW_OPENGL);
    Context = SDL_GL_CreateContext(Window);

    SDL_GL_SetSwapInterval(0);

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) 
    {
        std::cout << "LoaderGLLLoader did not work" << std::endl;
    }
    

    if(Context == NULL)
    {
        std::cout << "Context problem" << std::endl;
        return(1);
    }

    float positions[] = 
    {
        -0.5f, -0.5f,
         0.5f,  -0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f,
    }; 

    u32 indices[] = {
        0, 1, 2,
        // 2, 3, 0
    };
    u32 indices2[] = {
        // 0, 1, 2,
        2, 3, 0
    };

    /*
        Modern OpenGL requires a VAO be defined and bound if you are using the core profile.    
    */

    u32 VAO[2];
    u32 IBO[2];
    glGenBuffers(2, IBO);
    glGenVertexArrays(2, VAO);
    u32 BufferId;
    glGenBuffers(1, &BufferId);

    glBindBuffer(GL_ARRAY_BUFFER, BufferId);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(u32), indices, GL_STATIC_DRAW);

    // stride: bytes per vertex
    // pointer: pointer into attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(u32), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);


    ShaderProgramSource source = ParseShader("src/basic.shader");

    u32 shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /*
        once shader create (and mentions uniforms), it gets assigned an ID
        -1 if not used or not found
    */

    int location = glGetUniformLocation(shader, "u_Color");


    float r = 0.0f;
    float increment = 0.05;
    GetOpenGLInfo();
    while(is_running == true)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
            if(e.type == SDL_QUIT) is_running = false;

        // glDisable(GL_DEPTH_TEST);
        // glDisable(GL_CULL_FACE);
        // glViewport(0,0,Width,Height);

        glClearColor(0,0,0,1);
        glUniform4f(location, r, 0.3f, 0.8, 1.0f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        if(r > 1.0f)
        {
            increment = -increment;
        }
        else if(r < 0.0f)
        {
            increment = -increment;
        }
        r+=increment;



        // GetOpenGLInfo();

        SDL_GL_SwapWindow(Window);

    }


    return(0);
}