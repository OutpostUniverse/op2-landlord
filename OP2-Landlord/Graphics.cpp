#include "Graphics.h"


#include <SDL2/SDL_image.h>

#include <iostream>


Graphics::Graphics(ImVec2 windowSize) :
	mWindowSize{windowSize}
{
    init();
}


void Graphics::drawColor(const ImColor& color)
{
    SDL_SetRenderDrawColor(
        mRenderer,
        static_cast<Uint8>(color.Value.x * 255),
        static_cast<Uint8>(color.Value.y * 255),
        static_cast<Uint8>(color.Value.z * 255),
        static_cast<Uint8>(color.Value.w * 255)
    );
}


void Graphics::clear()
{
    SDL_RenderClear(mRenderer);
}


void Graphics::present()
{
    int w = 0, h = 0;
    SDL_GetWindowSize(mWindow, &w, &h);
    mWindowSize = { static_cast<float>(w), static_cast<float>(h) };

    SDL_RenderPresent(mRenderer);
}


Graphics::Texture Graphics::loadTexture(const std::string& filename)
{
    SDL_Surface* temp = IMG_Load(filename.c_str());
    if (!temp)
    {
        std::cout << "loadTexture(): Unable to load '" + filename + "': " + SDL_GetError() << std::endl;
        throw std::runtime_error("loadTexture(): Unable to load '" + filename + "': " + SDL_GetError());
    }

    SDL_Texture* out = SDL_CreateTextureFromSurface(mRenderer, temp);
    SDL_FreeSurface(temp);

    if (!out)
    {
        std::cout << "loadTexture(): Unable to load '" + filename + "': " + SDL_GetError() << std::endl;
        throw std::runtime_error("loadTexture(): Unable to load '" + filename + "': " + SDL_GetError());
    }

    int width = 0, height = 0;
    SDL_QueryTexture(out, nullptr, nullptr, &width, &height);

    return Texture{ out, SDL_Rect{ 0, 0, width, height }, { static_cast<float>(width), static_cast<float>(height) } };
}


void Graphics::init()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(std::string("Unable to initialize SDL: ") + SDL_GetError());
    }

    mWindow = SDL_CreateWindow(
        "OP2-Landlord",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        static_cast<int>(mWindowSize.x),
        static_cast<int>(mWindowSize.y),
        SDL_WINDOW_RESIZABLE);

    if (!mWindow)
    {
        throw std::runtime_error("initRenderer(): Unable to create primary window: " + std::string(SDL_GetError()));
    }

    SDL_SetWindowMinimumSize(mWindow, static_cast<int>(mWindowSize.x), static_cast<int>(mWindowSize.y));

    #if defined(__APPLE__)
    mWindowRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_SOFTWARE);
    #else
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    #endif

    if (!mRenderer)
    {
        throw std::runtime_error("initRenderer(): Unable to create renderer: " + std::string(SDL_GetError()));
    }

    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

    mWindowId = SDL_GetWindowID(mWindow);
}
