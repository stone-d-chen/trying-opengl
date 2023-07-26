#include "glad/glad.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <stdio.h>
#include <iostream>


#include <string>
typedef unsigned int u32;

    #include "IndexBuffer.cpp"
    #include "VertexBuffer.cpp"
    #include "VertexArray.h"
    #include "Shader.h"
    #include "Renderer.h"
    #include "Texture.h"

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
    if(SDL_Init(SDL_INIT_VIDEO) == 0) { is_running = true; }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    Window = SDL_CreateWindow("OGL Window", 0, 0, Width, Height, SDL_WINDOW_OPENGL);
    Context = SDL_GL_CreateContext(Window);

     SDL_GL_SetSwapInterval(1);

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) { std::cout << "LoaderGLLLoader did not work" << std::endl; }

    if(Context == NULL)
    {
        std::cout << "Context problem" << std::endl;
        return(1);
    }
  


    float positions[] = 
    {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f, 
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    }; 

    u32 indices[] = {
        0, 1, 2,
    };
    u32 indices2[] = {
        2, 3, 0
    };

    /*
        Modern OpenGL requires a VAO be defined and bound if you are using the core profile.    
    */


    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 3);


    VertexArray va2;
    va2.AddBuffer(vb, layout);
    IndexBuffer ib2(indices2, 3);

    Shader myshader("src/basic.shader");
    myshader.SetUniform4f("u_Color", 1.0f, 0.5f, 0.2f, 1.0f);
    Texture texture("src/wall.jpg");
    texture.Bind();
    myshader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05;

    GetOpenGLInfo();

    while(is_running == true)
    {
        // myshader.SetUniform4f("u_Color", r, 0.5, 0.2, 1.0);

        renderer.Draw(va, ib, myshader);
        renderer.Draw(va2, ib2, myshader);

        SDL_GL_SwapWindow(Window);


        if(r > 1.0f)
        {
            increment = -increment;
        }
        else if(r < 0.0f)
        {
            increment = -increment;
        }
        r+=increment;

        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
            if(e.type == SDL_QUIT) is_running = false;

    }

    return(0);
}