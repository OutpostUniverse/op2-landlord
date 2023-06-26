
#include <SDL2/SDL.h>

#include <filesystem>
#include <iostream>
#include <memory>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "EditorConfig.h"
#include "Events.h"
#include "FileIo.h"
#include "Graphics.h"
#include "Utility.h"


namespace
{
    Graphics graphics({ 800, 600 });
    FileIo fileIo(*graphics.window());

    constexpr auto ClearColor = NAS2D::Color{ 30, 30, 30, 255 };

    constexpr auto EmptyDir{ "Empty Directory" };
    constexpr auto EmptyDirMsg{ "No directory for Outpost 2 has been selected" };
    constexpr auto MissingAssets{ "Missing Assets" };
    constexpr auto MissingAssetsMsg{ "'Outpost2.exe' or 'art.vol' not found in specified directory." };

    EditorConfig Config(getUserPrefPath("OP2-Landlord", "OutpostUniverse"));

    bool InitialSetupRequired = false;
};


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


void doInitialSetup()
{
    ImGui::SetNextWindowSize({ 550, 170 });
    const ImVec2 position{
        static_cast<float>(graphics.size().x / 2 - 300),
        static_cast<float>(graphics.size().y / 2 - 85)
    };

    ImGui::SetNextWindowPos(position);

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Initial Setup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Location of Outpost 2");

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.90f);
    ImGui::InputText("##op2_path", Config.Op2FilePath, 500, ImGuiInputTextFlags_AutoSelectAll);
    ImGui::SameLine();
    if (ImGui::Button("...", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        if (fileIo.pickFolder())
        {
            std::ignore = strcpy_s(Config.Op2FilePath, fileIo.folderPath().c_str());
        }
    }

    ImGui::Dummy({ 0, 20 });

    if (ImGui::Button("Continue", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        const auto exepath = std::string(Config.Op2FilePath) + fileIo.pathSeparator() + "Outpost2.exe";
        const auto artpath = std::string(Config.Op2FilePath) + fileIo.pathSeparator() + "art.vol";
        const std::string path(trimWhitespace(Config.Op2FilePath));

        if (path.empty())
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, EmptyDir, EmptyDirMsg, graphics.window());
        }
        else if (!std::filesystem::exists(exepath) || !std::filesystem::exists(artpath))
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, MissingAssets, MissingAssetsMsg, graphics.window());
        }
        else
        {
            InitialSetupRequired = false;
        }
    }

    ImGui::End();
}


void mainLoop()
{
    std::string pathToOutpost2{};

    while (!quitMessagePosted())
    {
        pumpEvents();

        graphics.clear();

        guiNewFrame();
        
        if(InitialSetupRequired)
        {
            doInitialSetup();
        }
        else
        {

        }

        guiEndFrame();

        graphics.present();
    }
}


void initGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;
    ImGui::LoadIniSettingsFromDisk(Config.UserSavePath.c_str());

    std::ignore = io.Fonts->AddFontFromFileTTF("data/fonts/opensans.ttf", 24);

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(graphics.window(), graphics.renderer());
    ImGui_ImplSDLRenderer2_Init(graphics.renderer());
}


void checkConfig()
{
    InitialSetupRequired = !Config.Options.contains("op2_path");
}


int main(int argc, char* argv[])
{
    try
    {
        graphics.drawColor(ClearColor);
        initGui();

        checkConfig();
        
        mainLoop();

        ImGui::SaveIniSettingsToDisk(std::string{ Config.UserSavePath + "gui.ini" }.c_str());
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
