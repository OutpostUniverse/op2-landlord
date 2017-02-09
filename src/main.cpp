#include "NAS2D/NAS2D.h"

#include "EditorState.h"
#include "StartState.h"

#include "Defaults.h"


#ifdef WINDOWS
#include <Windows.h>
#endif

using namespace std;


int main(int argc, char *argv[])
{

	try
	{
		Game game("Landlord", argv[0], "editor.xml");
		game.mount("editor.zip");
		game.go(new StartState());
	}
	catch(Exception e)
	{
		stringstream errorMessage;
		errorMessage << "Error (" << e.getCode() << "):" << endl << endl << e.getDescription();
	
		#if defined(WINDOWS)
		MessageBoxA(NULL, errorMessage.str().c_str(), e.getBriefDescription().c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
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
