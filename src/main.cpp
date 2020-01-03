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


#ifdef WINDOWS
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif


int main(int argc, char *argv[])
{
	// redirect log entries on Release builds.
	FILE *stream;
	freopen_s(&stream, "log_editor.txt", "w", stdout);

	fillCellTypes();
	fillColorTables();

	try
	{
		Filesystem& f = Utility<Filesystem>::init<Filesystem>(argv[0], "OP2-Landlord", "Outpost Universe", "data");

		Configuration& cf = Utility<Configuration>::get();
		cf.load("config.xml");
		if (cf.graphicsWidth() < 1000 || cf.graphicsHeight() < 650)
		{
			cf.graphicsWidth(1000);
			cf.graphicsHeight(650);
		}
		cf.save();

		// All utilities must be initialized or exceptions are thrown when their Null
		// variants aren't in place.
		Utility<Mixer>::init<MixerNull>();

		Renderer& r = Utility<Renderer>::init<RendererOpenGL>("OP2-Landlord");
		r.addCursor("sys/normal.png", POINTER_NORMAL, 0, 0);
		r.addCursor("sys/fill.png", POINTER_FILL, 0, 0);
		r.addCursor("sys/eraser.png", POINTER_ERASE, 0, 0);
		r.showSystemPointer(true);
		r.minimum_size(1000, 650);
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
		std::stringstream errorMessage;
		errorMessage <<  e.what();
	
		#if defined(WINDOWS)
		MessageBoxA(NULL, errorMessage.str().c_str(), "Application Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif
		
	}
	catch(...)
	{
		#if defined(WINDOWS)
		MessageBoxA(NULL, "An unexpected error occured. Please yell at Lee about\nthis so he can fix it.\n\nAnd don't forget to include steps to reproduce and a\nlog! 'It crashed' will get you slapped.", "Unhandled Exception", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif
	}

	fclose(stream);
	return 0;
}
