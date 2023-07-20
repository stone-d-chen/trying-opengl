#include "include/SDL.h"
#include "include/SDL_opengl.h"
#include <stdio.h>
#include <iostream>

#define global static

global bool is_running = false;
global int Width = 640;
global int Height = 480;

SDL_Window *Window;
SDL_GLContext Context;

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
    if(Context == NULL)
    {
        std::cout << "Context problem" << std::endl;
        return(1);
    }

    while(is_running == true)
    {
        static int counter = 0;
        counter += 1;
        if(counter > 10000) is_running = false;
    }


    return(0);
}