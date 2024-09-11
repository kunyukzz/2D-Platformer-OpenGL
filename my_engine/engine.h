#pragma once

#include "glslprog.h"
#include "sprite.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <vector>
#include "camera2d.h"
#include "spritebatch.h"

enum class EngineState
{
    PLAY,
    EXIT
};

class Engine
{
    Window m_window;
    int m_scWdt;
    int m_scHgt;

    EngineState m_EState;

    void init();
    void initShader();
    void update();
    void input();
    void draw();
    void calcFPS();

    GLSLProgram m_shaderTest;
    Camera2D m_camera;

    SpriteBatch m_spriteBatch;

    float m_FPS;
    float m_maxFPS;
    float m_frameTime;
    float m_time;

  public:
    Engine();
    ~Engine();

    void run();
};
