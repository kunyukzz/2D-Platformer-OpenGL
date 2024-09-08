#pragma once

#include "sprite.h"
#include <SDL2/SDL.h>
#include <iostream>

enum class EngineState
{
    PLAY,
    EXIT
};

class Engine
{
    SDL_Window* m_window;
    // SDL_Renderer* m_renderer;
    int m_scHgt;
    int m_scWdt;

    EngineState m_EState;

    void init();
    void update();
    void input();
    void draw();

    Sprite m_sprite;

  public:
    Engine();
    ~Engine();

    void run();
};
