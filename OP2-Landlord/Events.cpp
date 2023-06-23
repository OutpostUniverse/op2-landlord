#include "Events.h"

#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"


namespace
{
    bool QuitMessageSet = false;
}


void pumpEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type)
        {
        case SDL_KEYDOWN:
            break;

        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            break;

        case SDL_QUIT:
            postQuitMessage();
            break;

        case SDL_WINDOWEVENT:
            break;

        default:
            break;
        }
    }
}


void postQuitMessage()
{
    QuitMessageSet = true;
}


bool quitMessagePosted()
{
    return QuitMessageSet;
}
