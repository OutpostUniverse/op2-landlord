#include "Gui.h"

#include <filesystem>

#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "FileIo.h"
#include "Utility.h"


Gui::Gui(StringTable& table, EditorConfig& config, Graphics& graphics, const std::string& settingsPath) :
    mStringTable{ table },
    mIniSavePath{ settingsPath },
    mGraphics{ graphics },
    mConfig{ config },
    mFileIo{ *graphics.window() }
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = nullptr;
    ImGui::LoadIniSettingsFromDisk(mIniSavePath.c_str());

    std::ignore = io.Fonts->AddFontFromFileTTF("data/fonts/opensans.ttf", 24);

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(mGraphics.window(), mGraphics.renderer());
    ImGui_ImplSDLRenderer2_Init(mGraphics.renderer());
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


/**
 * \return Returns true if this state should be called again
 */
Gui::AppState Gui::initialSetup()
{
    static char op2Path[1000] = { '\0' };

    ImGui::SetNextWindowSize({ 550, 170 });
    const ImVec2 position{
        static_cast<float>(mGraphics.size().x / 2 - 300),
        static_cast<float>(mGraphics.size().y / 2 - 85)
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
        if (mFileIo.pickFolder())
        {
            std::ignore = strcpy_s(op2Path, mFileIo.folderPath().c_str());
            mFileIo.setDefaultFolder(mFileIo.folderPath());
        }
    }

    ImGui::Dummy({ 0, 20 });

    AppState nextState = AppState::InitialSetup;

    if (ImGui::Button("Continue", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        const auto exepath = std::string(op2Path) + mFileIo.pathSeparator() + "Outpost2.exe";
        const auto artpath = std::string(op2Path) + mFileIo.pathSeparator() + "art.vol";
        const std::string path(trimWhitespace(op2Path));

        if (path.empty())
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                mStringTable[StringTable::StringName::EmptyDir].c_str(),
                mStringTable[StringTable::StringName::EmptyDirMsg].c_str(),
                mGraphics.window());
        }
        else if (!std::filesystem::exists(exepath) || !std::filesystem::exists(artpath))
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                mStringTable[StringTable::StringName::MissingAssets].c_str(),
                mStringTable[StringTable::StringName::MissingAssetsMsg].c_str(),
                mGraphics.window());
        }
        else
        {
            mConfig["Op2FilePath"] = op2Path;
            nextState = AppState::CreateOrLoadMap;
        }
    }

    ImGui::End();

    return nextState;
}


Gui::AppState Gui::createOrLoadMap()
{
    ImGui::SetNextWindowSize({ 550, 200 });
    const ImVec2 position{
        static_cast<float>(mGraphics.size().x / 2 - 300),
        static_cast<float>(mGraphics.size().y / 2 - 85)
    };

    ImGui::SetNextWindowPos(position);

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Start", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    AppState nextState = AppState::CreateOrLoadMap;

    if (ImGui::Button("Load Map from Disk", { ImGui::GetContentRegionAvail().x, 0 }))
    { 
        mFileIo.setFilter(0);
        if (mFileIo.pickOpenFile())
        {
            nextState = AppState::LoadMap;
        }
    }

    if (ImGui::Button("Load Map from Archive", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        mFileIo.setFilter(1);
        if (mFileIo.pickOpenFile())
        {
            
        }
    }

    if (ImGui::Button("New Map", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        nextState = AppState::NewMap;
    }

    ImGui::Dummy({ 0, 5 });
    ImGui::Separator();
    ImGui::Dummy({ 0, 5 });
    
    if (ImGui::Button("Quit", { ImGui::GetContentRegionAvail().x, 0 }))
    {
        nextState = AppState::Quit;
    }

    ImGui::End();

    return nextState;
}
