
#include <SDL2/SDL.h>

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>

#if defined(_WIN32)
#define NOMINMAX
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

#include "OP2Utility.h"


using namespace OP2Utility;


namespace
{
    constexpr auto ClearColor = ImColor{ 0.117f, 0.117f, 0.117f, 1.0f };

    Gui::AppState ApplicationState{ Gui::AppState::InitialSetup };

    using StateGuiFunction = std::function<Gui::AppState(void)>;
    std::map<Gui::AppState, StateGuiFunction> StateFunctionTable;

    using StateTransitionFunction = std::function<void(EditorConfig&, Graphics&, Gui&)>;
    std::map<Gui::AppState, StateTransitionFunction> StateTransitionFunctionTable;

    void dummyTransition(EditorConfig&, Graphics&, Gui&) {}

    std::vector<Graphics::Texture> TileSets;
};


void mainLoop(EditorConfig& config, Graphics& graphics, Gui& gui)
{
    std::string pathToOutpost2{};

    while (!quitMessagePosted())
    {
        pumpEvents();

        graphics.clear();
        gui.newFrame();
        
        const auto previousState = ApplicationState;
        ApplicationState = StateFunctionTable.at(ApplicationState)();

        if (ApplicationState == Gui::AppState::Quit)
        {
            return;
        }

        if (previousState != ApplicationState)
        {
            try
            {
                StateTransitionFunctionTable.at(ApplicationState)(config, graphics, gui);
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


Graphics::Texture bmpToTexture(Graphics& graphics, EditorConfig& config, BitmapFile& bmp)
{
    std::size_t pixelOffset = sizeof(BmpHeader) + sizeof(ImageHeader) + bmp.palette.size() * sizeof(Color);
    std::size_t bufferSize = pixelOffset + ImageHeader::CalculatePitch(bmp.imageHeader.bitCount, bmp.imageHeader.width) * std::abs(bmp.imageHeader.height);

    auto buffer = new uint8_t[bufferSize];
    memset(buffer, 0, bufferSize);

    Stream::MemoryWriter writer(buffer, bufferSize);

    bmp.WriteIndexed(writer);

    return graphics.loadTexturePacked(buffer, bufferSize);
}


void loadOrCreateTransition(EditorConfig& config, Graphics& graphics, Gui& gui)
{
    Archive::VolFile artVol(config["Op2FilePath"] + "/" + "art.vol");
    const auto indexCount = artVol.GetCount();

    std::vector<size_t> tilesetIndicies;
    for (size_t i = 0; i < indexCount; ++i)
    {
        const auto artName{ StringUtility::ConvertToUpper(artVol.GetName(i)) };

        if (artName.find("WELL") != std::string::npos)
        {
            tilesetIndicies.push_back(i);
        }
    }

    for (size_t i = 0; i < tilesetIndicies.size(); ++i)
    {
        const auto tsetIndex = tilesetIndicies[i];

        try
        {
            auto bitmap = Tileset::ReadTileset(*artVol.OpenStream(tsetIndex));
            TileSets.push_back(bmpToTexture(graphics, config, bitmap));          
        }
        catch(std::runtime_error e)
        {
            std::cout << "[Warning] Unable to load tilset '" << artVol.GetName(tsetIndex) << "' : " << e.what() << std::endl;
        }
    }
}


void buildStateGuiFunctionTable(Gui& gui)
{
    StateFunctionTable.emplace(Gui::AppState::InitialSetup, [&gui]() { return gui.initialSetup(); });
    StateFunctionTable.emplace(Gui::AppState::CreateOrLoadMap, [&gui]() { return gui.createOrLoadMap(); });
    StateFunctionTable.emplace(Gui::AppState::NewMap, [&gui]() { return gui.dummy(); });

    StateTransitionFunctionTable.emplace(Gui::AppState::CreateOrLoadMap, loadOrCreateTransition);
    StateTransitionFunctionTable.emplace(Gui::AppState::CreateOrLoadMap, dummyTransition);
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
            loadOrCreateTransition(config, graphics, gui);
        }
        
        mainLoop(config, graphics, gui);

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
