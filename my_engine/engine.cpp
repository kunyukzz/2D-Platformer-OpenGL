#include "engine.h"
#include "resmanager.h"
// #include <iostream>

// engine parameter initialization list
Engine::Engine()
    : m_scWdt(800)
    , m_scHgt(600)
    , m_EState(EngineState::PLAY)
    , m_maxFPS(60.0f)
    , m_time(0.0f)
{
    m_camera.init(m_scWdt, m_scHgt);
};

Engine::~Engine() {}

void Engine::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    // set SDL to a double buffer window so screen dont get any flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window.create("Game Engine", m_scWdt, m_scHgt, 0);
    initShader();

    m_spriteBatch.init();
}

void Engine::initShader()
{
    m_shaderTest.compAllShader("shaders/colorShading.vert", "shaders/colorShading.frag");
    m_shaderTest.addAttr("vertexPosition");
    m_shaderTest.addAttr("vertexColor");
    m_shaderTest.addAttr("vertexUV");
    m_shaderTest.linkShader();
}

void Engine::update()
{
    while (m_EState != EngineState::EXIT)
    {
        // float startTick = SDL_GetTicks();

        input();
        m_time += 0.01;

        m_camera.update();
        draw();
        // calcFPS();

        // print only once every 10 frames
        /* static int frameCounter = 0;
        frameCounter++;
        if (frameCounter == 10)
        {
            std::cout << m_FPS << "\n";
            frameCounter = 0;
        }

        float frameTick = SDL_GetTicks() - startTick; */

        // THIS IS CURRENTLY NOT WORKING AND I DON'T KNOW HOW TO FIX IT!!!!!!
        // limit FPS to max FPS
        // if (1000.0f / m_maxFPS)
        //    SDL_Delay(1000.0f / m_maxFPS - frameTick);
    }
}

void Engine::input()
{
    SDL_Event ev;
    const float CAMSPEED = 20.0f;
    const float ZOOMSPEED = 0.1f;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT: // window 'x' button
            m_EState = EngineState::EXIT;
            break;

            // case SDL_MOUSEMOTION:
            //     std::cout << ev.motion.x << " " << ev.motion.y << "\n";
            //     break;

        case SDL_KEYDOWN: // check if keyboard button being pressed
            switch (ev.key.keysym.sym)
            {
            case SDLK_ESCAPE: // keyboard escape button
                m_EState = EngineState::EXIT;
                break;
            case SDLK_w:
                m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0, CAMSPEED));
                break;
            case SDLK_s:
                m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0, -CAMSPEED));
                break;
            case SDLK_a:
                m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMSPEED, 0.0f));
                break;
            case SDLK_d:
                m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMSPEED, 0.0f));
                break;
            case SDLK_q:
                m_camera.setScale(m_camera.getScale() + ZOOMSPEED);
                break;
            case SDLK_e:
                m_camera.setScale(m_camera.getScale() - ZOOMSPEED);
                break;
            }

        default:
            break;
        }
    }
}

void Engine::run()
{
    init();
    update();
}

void Engine::draw()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderTest.use();
    glActiveTexture(GL_TEXTURE0);

    GLint texturelocation = m_shaderTest.getUniformLoc("testSampler");
    glUniform1i(texturelocation, 0);

    GLuint timeLoc = m_shaderTest.getUniformLoc("time");
    glUniform1f(timeLoc, m_time);

    // set camera matrix
    GLuint pLoc = m_shaderTest.getUniformLoc("P");
    glm::mat4 cameraMatrix = m_camera.getCamMatrix();

    // update matrix to GPU
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, &(cameraMatrix[0][0]));

    m_spriteBatch.begin();
    glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static GLTexture texture = ResourceManager::getTexture("assets/images/test.png");
    Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;

    m_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
    m_spriteBatch.draw(pos + glm::vec4(50, 0, 0, 0), uv, texture.id, 0.0f, color);

    m_spriteBatch.end();
    m_spriteBatch.renderBatch();

    glBindTexture(GL_TEXTURE_2D, 0);
    m_shaderTest.unused();

    m_window.swapBuffer();
}

void Engine::calcFPS()
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
