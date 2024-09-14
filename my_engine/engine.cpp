#include "engine.h"
#include "resmanager.h"
#include <iostream>

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

    m_window.create("Game Engine", m_scWdt, m_scHgt, DEFAULT);

    initShader();

    m_spriteBatch.init();
    m_fpsLimiter.init(m_maxFPS);
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
        m_fpsLimiter.begin();
        input();
        m_time += 0.01;
        m_camera.update();
        draw();

        // print only once every 10 frames
        m_FPS = m_fpsLimiter.end();
        static int frameCounter = 0;
        frameCounter++;
        if (frameCounter % 10 == 0)
        {
            std::cout << m_FPS << "\n";
        }
    }
}

void Engine::input()
{
    SDL_Event ev;
    const float CAMSPEED = 2.0f;
    const float ZOOMSPEED = 0.1f;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT: // window 'x' button
            m_EState = EngineState::EXIT;
            break;
        case SDL_KEYDOWN: // check if keyboard button being pressed
            m_inputManager.pressKey(ev.key.keysym.sym);
            break;
        case SDL_KEYUP:
            m_inputManager.releasKey(ev.key.keysym.sym);
            break;
        default:
            break;
        }
    }

    if (m_inputManager.isKeyPressed(SDLK_ESCAPE))
        m_EState = EngineState::EXIT;

    if (m_inputManager.isKeyPressed(SDLK_w))
        m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0, CAMSPEED));

    if (m_inputManager.isKeyPressed(SDLK_s))
        m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0, -CAMSPEED));

    if (m_inputManager.isKeyPressed(SDLK_a))
        m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMSPEED, 0.0f));

    if (m_inputManager.isKeyPressed(SDLK_d))
        m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMSPEED, 0.0f));

    if (m_inputManager.isKeyPressed(SDLK_q))
        m_camera.setScale(m_camera.getScale() + ZOOMSPEED);

    if (m_inputManager.isKeyPressed(SDLK_e))
        m_camera.setScale(m_camera.getScale() - ZOOMSPEED);
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
    //m_spriteBatch.draw(pos + glm::vec4(50, 0, 0, 0), uv, texture.id, 0.0f, color);

    m_spriteBatch.end();
    m_spriteBatch.renderBatch();

    glBindTexture(GL_TEXTURE_2D, 0);
    m_shaderTest.unused();

    m_window.swapBuffer();
}
