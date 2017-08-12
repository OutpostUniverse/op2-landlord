#include "NAS2D/NAS2D.h"

#include "EditorState.h"
#include "StartState.h"

#include "Common.h"
#include "Defaults.h"

#include "Map/color_table.h"

#include <map>
#include <sstream>


#ifdef WINDOWS
#include <Windows.h>
#endif

using namespace std;

int main(int argc, char *argv[])
{
	// redirect log entries on Release builds.
	#ifndef _DEBUG
	FILE *stream;
	freopen_s(&stream, "log_editor.txt", "w", stdout);
	#endif

	fillColorTables();

	try
	{
		Game game("Landlord", argv[0], "editor.xml");
		game.mount("editor.zip");

		Renderer& r = Utility<Renderer>::get();
		r.addCursor("sys/normal.png", POINTER_NORMAL, 0, 0);
		r.addCursor("sys/fill.png", POINTER_FILL, 0, 0);
		r.addCursor("sys/eraser.png", POINTER_ERASE, 0, 0);
		r.showSystemPointer(true);
		r.minimum_size(800, 600);
		r.resizeable(true);

		game.go(new StartState());
		//game.go(new EditorState("maps/mp6_01.map"));
	}
	catch(std::runtime_error& e)
	{
		stringstream errorMessage;
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

	return 0;
}
