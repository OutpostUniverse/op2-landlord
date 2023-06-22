
#include <SDL2/SDL.h>

#include <iostream>
#include <memory>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "Graphics.h"


#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


namespace
{
    bool Exit = false;

    std::shared_ptr<Graphics> graphics;

    constexpr auto ClearColor = NAS2D::Color{ 30, 30, 30, 255 };
};


void pumpEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type)
        {
        case SDL_KEYDOWN:
            Exit = event.key.keysym.sym == SDLK_ESCAPE;
            break;

        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            break;

        case SDL_QUIT:
            Exit = true;
            break;

        case SDL_WINDOWEVENT:
            break;

        default:
            break;
        }
    }
}


void guiEndFrame()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}


void guiNewFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}


void mainLoop()
{
    std::string pathToOutpost2{};

    while (!Exit)
    {
        pumpEvents();

        graphics->clear();

        guiNewFrame();

        ImGui::SetNextWindowSize({ 500, 500 });
        const ImVec2 position{
            static_cast<float>(graphics->size().x / 2 - 250),
            static_cast<float>(graphics->size().y / 2 - 250)
        };
        
        ImGui::SetNextWindowPos(position);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Initial Setup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

        ImGui::Text("Select the Outpost 2 folder");

        ImGui::End();

        guiEndFrame();

        graphics->present();
    }
}


void initGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    std::ignore = io.Fonts->AddFontFromFileTTF("data/fonts/opensans.ttf", 24);
 

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(graphics->window(), graphics->renderer());
    ImGui_ImplSDLRenderer2_Init(graphics->renderer());
}


int main(int argc, char* argv[])
{
    try
    {
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            throw std::runtime_error(std::string("Unable to initialize SDL: ") + SDL_GetError());
        }

        graphics = std::make_shared<Graphics>(NAS2D::Vector<int>{ 800, 600 });
        graphics->drawColor(ClearColor);

        initGui();

        mainLoop();

        SDL_Quit();
    }
    catch (std::exception e)
    {
        std::string message(std::string(e.what()) + "\n\nOP2-Landlord will now close.");

        #if defined(_WIN32)
        MessageBoxA(nullptr, message.c_str(), "OP2-Landlord", MB_ICONERROR | MB_OK);
        #else
        std::cout << message << std::endl;
        #endif
    }

	return 0;
}
