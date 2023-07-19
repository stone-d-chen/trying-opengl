#include "include/SDL.h"
#include <stdio.h>

#define global_variable static

global_variable bool is_running = false;

int main(int ArgC, char **Args)
{
    printf("start\n");
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        is_running = true;
        printf("running");
    }

    while(is_running == true)
    {
    }


    return(0);
}