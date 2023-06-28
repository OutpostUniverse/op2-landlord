#include "Gui.h"

#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


Gui::Gui(const std::string& settingsPath, SDL_Window* window, SDL_Renderer* renderer) :
    mIniSavePath{ settingsPath }
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;
    ImGui::LoadIniSettingsFromDisk(mIniSavePath.c_str());

    std::ignore = io.Fonts->AddFontFromFileTTF("data/fonts/opensans.ttf", 24);

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}


Gui::~Gui()
{
    ImGui::SaveIniSettingsToDisk(mIniSavePath.c_str());
}


void Gui::newFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}


void Gui::endFrame()
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
