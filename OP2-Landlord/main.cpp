
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


void GameLoop()
{
    bool show_demo_window = false;
    bool show_another_window = false;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO& io = ImGui::GetIO();

    while (!Exit)
    {

        pumpEvents();

        graphics->clear();


        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();



        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))
        {
            counter++;
        }

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();



        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());


        graphics->present();
    }
}


void initGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
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

        GameLoop();

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
