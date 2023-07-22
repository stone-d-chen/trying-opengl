#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <stdio.h>
#include <iostream>
typedef unsigned int u32;

#define global static


global bool is_running = false;
global int Width = 640;
global int Height = 480;

SDL_Window *Window;
SDL_GLContext Context;


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
    u32 vs = CompileShader (GL_VERTEX_SHADER, vertexShader);
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

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) 
    {
        std::cout << "LoaderGLLLoader did not work" << std::endl;
    }
    

    if(Context == NULL)
    {
        std::cout << "Context problem" << std::endl;
        return(1);
    }

    float positions[8] = 
    {
        -0.5f, -0.5f,
         0.5f,  -0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f,
    }; 

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    /*
    Modern OpenGL requires a VAO be defined and bound if you are using the core profile.
    Add *code below* to your code,
    otherwise you will draw nothing.
    
    */

    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    u32 BufferId;
    glGenBuffers(1, &BufferId);
    glBindBuffer(GL_ARRAY_BUFFER, BufferId);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    u32 IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), indices, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


    // glBindBuffer(GL_ARRAY_BUFFER, 0);


    std::string vertexShader = 
    "#version 330 core\n"
    "\n"
    "in vec4 position;\n"
    "\n"
    "void main()\n"
    "{\n"
        "gl_Position = position;\n"
    "}\n";

    std::string fragmentShader = 
    "#version 330 core\n"
    "\n"
    "out vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

    u32 shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);



    // stride: bytes per vertex
    // pointer: pointer into attribute





    GetOpenGLInfo();
    while(is_running == true)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
            if(e.type == SDL_QUIT) is_running = false;

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glViewport(0,0,Width,Height);

        glClearColor(1,0,1,1);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // GetOpenGLInfo();

        SDL_GL_SwapWindow(Window);

    }


    return(0);
}