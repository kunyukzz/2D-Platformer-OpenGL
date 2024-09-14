#pragma once

#include "camera2d.h"
#include "glslprog.h"
#include "inputmanager.h"
#include "sprite.h"
#include "spritebatch.h"
#include "timing.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <vector>

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

    GLSLProgram m_shaderTest;
    Camera2D m_camera;
    SpriteBatch m_spriteBatch;
    InputManager m_inputManager;
    FPSLimiter m_fpsLimiter;

    float m_FPS;
    float m_maxFPS;
    float m_time;

  public:
    Engine();
    ~Engine();

    void run();
};
