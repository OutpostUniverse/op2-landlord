
#include <SDL2/SDL.h>

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
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

    Gui::AppState ApplicationState{ Gui::AppState::InitialSetup };

    using StateGuiFunction = std::function<Gui::AppState(void)>;
    std::map<Gui::AppState, StateGuiFunction> StateFunctionTable;

    using StateTransitionFunction = std::function<void(Gui&)>;
    std::map<Gui::AppState, StateTransitionFunction> StateTransitionFunctionTable;
};


void mainLoop(Graphics& graphics, Gui& gui)
{
    std::string pathToOutpost2{};

    while (!quitMessagePosted())
    {
        pumpEvents();

        graphics.clear();
        gui.newFrame();
        
        const auto previousState = ApplicationState;
        ApplicationState = StateFunctionTable.at(ApplicationState)();

        if (previousState != ApplicationState)
        {
            try
            {
                StateTransitionFunctionTable.at(ApplicationState)(gui);
            }
            catch (std::out_of_range)
            {
                std::cout << "[Warning] No transition handler for ApplicationState '" << static_cast<int>(ApplicationState) << "'" << std::endl;
            }
        }

        gui.endFrame();
        graphics.present();
    }
}


void checkConfig(EditorConfig& config)
{
    ApplicationState = config.contains("Op2FilePath") ? Gui::AppState::CreateOrLoadMap : Gui::AppState::InitialSetup;
}


void loadOrCreateTransition(Gui& gui)
{
}


void buildStateGuiFunctionTable(Gui& gui)
{
    StateFunctionTable.emplace(Gui::AppState::InitialSetup, [&gui]() { return gui.initialSetup(); });
    StateFunctionTable.emplace(Gui::AppState::CreateOrLoadMap, [&gui]() { return gui.createOrLoadMap(); });

    StateTransitionFunctionTable.emplace(Gui::AppState::CreateOrLoadMap, loadOrCreateTransition);
}


int main(int argc, char* argv[])
{
    try
    {
        StringTable strings{ "data/en.json" };
        
        Graphics graphics({ 800, 600 });
        graphics.drawColor(ClearColor);

        EditorConfig config(getUserPrefPath("OP2-Landlord", "OutpostUniverse"));
        Gui gui(strings, config, graphics, config["UserSavePath"] + "gui.ini");

        buildStateGuiFunctionTable(gui);
        checkConfig(config);

        if (ApplicationState != Gui::AppState::InitialSetup)
        {
            loadOrCreateTransition(gui);
        }
        
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
