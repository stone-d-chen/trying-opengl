@echo off
cl.exe /O2 sdl_ogl.cpp  /link lib\SDL2main.lib lib\SDL2.lib "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64\shell32.lib" /SUBSYSTEM:CONSOLE /OUT:lib/sdl_ogl.exe