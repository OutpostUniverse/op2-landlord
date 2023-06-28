
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
    constexpr auto ClearColor = ImColor{ 0.117f, 0.117f, 0.117f, 1.0f };
    bool InitialSetupRequired = false;
};


void mainLoop(Graphics& graphics, Gui& gui)
{
    std::string pathToOutpost2{};

    while (!quitMessagePosted())
    {
        pumpEvents();

        graphics.clear();

        gui.newFrame();
        
        if(InitialSetupRequired)
        {
            InitialSetupRequired = gui.initialSetup();
        }
        else
        {

        }

        gui.endFrame();

        graphics.present();
    }
}


void checkConfig(EditorConfig& config)
{
    InitialSetupRequired = !config.contains("Op2FilePath");
}


int main(int argc, char* argv[])
{
    try
    {
        StringTable MessageStrings;        
        MessageStrings.load("data/en.json");
        
        Graphics graphics({ 800, 600 });
        graphics.drawColor(ClearColor);

        EditorConfig config(getUserPrefPath("OP2-Landlord", "OutpostUniverse"));

        Gui gui(MessageStrings, config, graphics, config["UserSavePath"] + "gui.ini");

        checkConfig(config);
        
        mainLoop(graphics, gui);

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
