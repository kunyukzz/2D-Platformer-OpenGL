#include "engine.h"

void hdlError(std::string error)
{
    std::cout << error << "\n";
    SDL_Quit();
}

Engine::Engine()
{
    m_window = NULL;
    m_scWdt = 800;
    m_scHgt = 600;
    m_EState = EngineState::PLAY;
}

Engine::~Engine() {}

void Engine::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    m_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_scWdt, m_scHgt, SDL_WINDOW_OPENGL);
    if (m_window == NULL)
        hdlError("Window cannot initialize!");

    SDL_GLContext glContex = SDL_GL_CreateContext(m_window);
    if (glContex == NULL)
        hdlError("GL_Context cannot created!");

    // glewExperimental = true;
    GLenum error = glewInit();
    if (error != GLEW_OK)
        hdlError("Cannot initialize glew");

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Engine::update()
{
    while (m_EState != EngineState::EXIT)
    {
        input();
        draw();
    }
}

void Engine::input()
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT: // window 'x' button
            m_EState = EngineState::EXIT;
            break;

        case SDL_MOUSEMOTION:
            std::cout << ev.motion.x << " " << ev.motion.y << "\n";
            break;

        case SDL_KEYDOWN: // check if keyboard button being pressed
            switch (ev.key.keysym.sym)
            {
            case SDLK_ESCAPE: // keyboard escape button
                m_EState = EngineState::EXIT;
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
    m_sprite.init(-1.0f, -1.0f, 1.0f, 1.0f);
    update();
}

void Engine::draw()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_sprite.draw();

    SDL_GL_SwapWindow(m_window);
}
