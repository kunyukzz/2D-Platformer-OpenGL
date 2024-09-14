#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

enum WindowFlags
{
    INVINSIBLE = 0x1,
    FULLSCREEN = 0x2,
    BORDERLESS = 0x4,
    DEFAULT = 0x8
};

class Window
{
    SDL_Window* m_sdlWindow;
    int m_screenW, m_screenH;

  public:
    Window();
    ~Window();

    int create(std::string windowName, int scrWidth, int scrHeight, unsigned int currentFlags);

    void swapBuffer();

    int getScrWidth() { return m_screenW; }
    int getScrHeight() { return m_screenH; }
};
