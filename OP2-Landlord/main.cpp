
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
#include "Gui.h"
#include "StringTable.h"
#include "Utility.h"


namespace
{
    Graphics graphics({ 800, 600 });
    FileIo fileIo(*graphics.window());

    constexpr auto ClearColor = ImColor{ 0.117f, 0.117f, 0.117f, 1.0f };

    EditorConfig Config(getUserPrefPath("OP2-Landlord", "OutpostUniverse"));
    StringTable MessageStrings;

    bool InitialSetupRequired = false;
};


void doInitialSetup()
{
    static char op2Path[1000] = { '\0' };

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
    ImGui::InputText("##op2_path", op2Path, 500, ImGuiInputTextFlags_AutoSelectAll);
    ImGui::SameLine();
    if (ImGui::Button("...", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        if (fileIo.pickFolder())
        {
            std::ignore = strcpy_s(op2Path, fileIo.folderPath().c_str());
        }
    }

    ImGui::Dummy({ 0, 20 });

    if (ImGui::Button("Continue", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        const auto exepath = std::string(op2Path) + fileIo.pathSeparator() + "Outpost2.exe";
        const auto artpath = std::string(op2Path) + fileIo.pathSeparator() + "art.vol";
        const std::string path(trimWhitespace(op2Path));

        if (path.empty())
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                MessageStrings[StringTable::StringName::EmptyDir].c_str(),
                MessageStrings[StringTable::StringName::EmptyDirMsg].c_str(),
                graphics.window());
        }
        else if (!std::filesystem::exists(exepath) || !std::filesystem::exists(artpath))
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                MessageStrings[StringTable::StringName::MissingAssets].c_str(),
                MessageStrings[StringTable::StringName::MissingAssetsMsg].c_str(),
                graphics.window());
        }
        else
        {
            Config["Op2FilePath"] = op2Path;
            InitialSetupRequired = false;
        }
    }

    ImGui::End();
}


void mainLoop(Gui& gui)
{
    std::string pathToOutpost2{};

    while (!quitMessagePosted())
    {
        pumpEvents();

        graphics.clear();

        gui.newFrame();
        
        if(InitialSetupRequired)
        {
            doInitialSetup();
        }
        else
        {

        }

        gui.endFrame();

        graphics.present();
    }
}


void checkConfig()
{
    InitialSetupRequired = !Config.contains("Op2FilePath");
}


int main(int argc, char* argv[])
{
    try
    {
        MessageStrings.load("data/en.json");
        graphics.drawColor(ClearColor);

        Gui gui(Config["UserSavePath"] + "gui.ini", graphics.window(), graphics.renderer());

        checkConfig();
        
        mainLoop(gui);

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
