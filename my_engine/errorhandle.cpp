#include "errorhandle.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

void hdlError(std::string error)
{
    std::cout << error << "\n";
    SDL_Quit();
    exit(50);
}

