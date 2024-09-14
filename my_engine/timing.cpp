#include "timing.h"
#include <SDL2/SDL.h>

void FPSLimiter::calcFPS()
{
    static const int s_numSample = 10;
    static float frameTimes[s_numSample];
    static int currentFrame = 0;

    static float prevTick = SDL_GetTicks();
    float currentTick;

    currentTick = SDL_GetTicks();

    m_frameTime = currentTick - prevTick;
    frameTimes[currentFrame % s_numSample] = m_frameTime;

    prevTick = currentTick;
    int count;
    currentFrame++;

    // get count of sample
    if (currentFrame < s_numSample)
        count = currentFrame;
    else
        count = s_numSample;

    // calculate average frime time
    float frameTimeAvg = 0;
    for (int i = 0; i < count; i++)
    {
        frameTimeAvg += frameTimes[i];
    }
    frameTimeAvg /= count;

    // calculate FPS
    if (frameTimeAvg > 0)
        m_FPS = 1000.0f / frameTimeAvg;
    else
        m_FPS = 60.0f;
}

FPSLimiter::FPSLimiter() {}

void FPSLimiter::init(float targetFPS)
{
    setTargetFps(targetFPS);
}

void FPSLimiter::setTargetFps(float targetFPS)
{
    m_maxFPS = targetFPS;
}

void FPSLimiter::begin()
{
    m_startTicks = SDL_GetTicks();
}

float FPSLimiter::end()
{
    calcFPS();

    float frameTick = SDL_GetTicks() - m_startTicks;
    float frameDelay = 1000.0f / m_maxFPS;

    if (frameDelay > frameTick)
        SDL_Delay(frameDelay - frameTick);

    return m_FPS;
}
