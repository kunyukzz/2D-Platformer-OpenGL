#pragma once

class FPSLimiter
{
    float m_targetFPS;
    unsigned int m_startTicks;

    float m_FPS;
    float m_maxFPS;
    float m_frameTime;

    void calcFPS();

  public:
    FPSLimiter();

    void init(float targetFPS);
    void setTargetFps(float targetFPS);
    void begin();

    float end();
};
