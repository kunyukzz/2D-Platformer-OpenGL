#include "window.h"
#include "errorhandle.h"

Window::Window() {}

Window::~Window() {}

int Window::create(std::string windowName, int scrWidth, int scrHeight, unsigned int currentFlags)
{
    Uint32 flags = SDL_WINDOW_OPENGL;
    if (currentFlags & INVINSIBLE)
        flags |= SDL_WINDOW_HIDDEN;
    if (currentFlags & FULLSCREEN)
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (currentFlags & BORDERLESS)
        flags |= SDL_WINDOW_BORDERLESS;
    if (currentFlags & DEFAULT)
        flags |= SDL_WINDOW_OPENGL;

    // set SDL to a double buffer window so screen dont get any flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scrWidth, scrHeight, flags);
    if (m_sdlWindow == NULL)
        hdlError("Window cannot initialize!");

    SDL_GLContext glContex = SDL_GL_CreateContext(m_sdlWindow);
    if (glContex == NULL)
        hdlError("GL_Context cannot created!");

    GLenum error = glewInit();
    if (error != GLEW_OK)
        hdlError("Cannot initialize glew");

    // check OpenGL version
    std::printf("*** OpenGL Version: %s ***", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // color background

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void Window::swapBuffer()
{

    SDL_GL_SwapWindow(m_sdlWindow);
}
