#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <stdio.h>
#include <iostream>


#define global static

global bool is_running = false;
global int Width = 640;
global int Height = 480;

SDL_Window *Window;
SDL_GLContext Context;

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
    
    GetOpenGLInfo();

    if(Context == NULL)
    {
        std::cout << "Context problem" << std::endl;
        return(1);
    }

    while(is_running == true)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
            if(e.type == SDL_QUIT) is_running = false;


    }


    return(0);
}