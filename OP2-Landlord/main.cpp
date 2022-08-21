#include <NAS2D/NAS2D.h>
#include <NAS2D/Renderer/RendererOpenGL.h>
#include <NAS2D/Mixer/MixerNull.h>

#include "EditorState.h"
#include "StartState.h"

#include "Common.h"
#include "Defaults.h"

#include "Map/cell_types.h"
#include "Map/color_table.h"

#include <map>
#include <sstream>
#include <iostream>
#include <fstream>


#ifdef WINDOWS
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif


int main(int argc, char *argv[])
{
	// Save for later
	std::streambuf* originalCoutBuffer = std::cout.rdbuf();
	// Redirect output to log file
	std::ofstream logFile("log_editor.txt");
	std::cout.rdbuf(logFile.rdbuf());

	fillCellTypes();
	fillColorTables();

	try
	{
		Filesystem& f = Utility<Filesystem>::init<Filesystem>("OP2-Landlord", "Outpost Universe");
		f.mount("data");
		f.mountReadWrite(f.prefPath());

		Configuration& cf = Utility<Configuration>::init(
			std::map<std::string, Dictionary>{
				{
					"graphics",
					{{
						{"screenwidth", 1000},
						{"screenheight", 650},
						{"fullscreen", false},
						{"vsync", true},
					}}
				}
			}
		);
		cf.load("config.xml");

		auto& graphics = cf["graphics"];
		if (graphics.get<int>("screenwidth") < 1000 || graphics.get<int>("screenheight") < 650)
		{
			graphics.set("screenwidth", 1000);
			graphics.set("screenheight", 650);
		}
		cf.save("config.xml");

		// All utilities must be initialized or exceptions are thrown when their Null
		// variants aren't in place.
		Utility<Mixer>::init<MixerNull>();

		Renderer& r = Utility<Renderer>::init<RendererOpenGL>("OP2-Landlord");
		r.addCursor("sys/normal.png", POINTER_NORMAL, 0, 0);
		r.addCursor("sys/fill.png", POINTER_FILL, 0, 0);
		r.addCursor("sys/eraser.png", POINTER_ERASE, 0, 0);
		r.showSystemPointer(true);
		r.minimumSize({1000, 650});
		r.resizeable(true);

		StateManager stateManager;
		stateManager.setState(new StartState());

		while (stateManager.update())
		{
			Utility<Renderer>::get().update();
		}

	}
	catch(std::runtime_error& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		#if defined(WINDOWS)
		MessageBoxA(NULL, e.what(), "Application Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif
	}

	// Reset to stdout again (prevents crashes on exit)
	std::cout.rdbuf(originalCoutBuffer);

	return 0;
}
