@echo off
cl.exe -Zi src/sdl_ogl.cpp src/glad.c imgui_impl_sdl2.cpp  imgui_impl_opengl3.cpp imgui*.cpp -I .\include /link lib\SDL2main.lib lib\SDL2.lib opengl32.lib "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64\shell32.lib" /SUBSYSTEM:CONSOLE /OUT:lib/sdl_ogl.exe
