#include "NAS2D/NAS2D.h"

#include "EditorState.h"
#include "StartState.h"


#include "ConfigStrings.h"
#include "Defaults.h"


using namespace std;


void validateOptions();


int main(int argc, char *argv[])
{

	try
	{
		Game game("Landlord", argv[0], "editor.xml");
		validateOptions();
		game.mount("editor.zip");
		game.go(new StartState());
	}
	catch(Exception e)
	{
		stringstream errorMessage;
		errorMessage << "Error (" << e.getCode() << "):" << endl << endl << e.getDescription();
	
		#if defined(WIN32)
		MessageBoxA(NULL, errorMessage.str().c_str(), e.getBriefDescription().c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif
		
	}
	catch(...)
	{
	}

	return 0;
}



void validateOptions()
{
	Configuration& c = Utility<Configuration>::get();

	if(c.option(CONFIG_UI_BUTTON_CREATE_NEW_MAP).empty())
		c.option(CONFIG_UI_BUTTON_CREATE_NEW_MAP, UI_BUTTON_CREATE_NEW_MAP);

	if(c.option(CONFIG_UI_BUTTON_LOAD_EXISTING_MAP).empty())
		c.option(CONFIG_UI_BUTTON_LOAD_EXISTING_MAP, UI_BUTTON_LOAD_EXISTING_MAP);

	if(c.option(CONFIG_UI_BUTTON_REFRESH_LISTS).empty())
		c.option(CONFIG_UI_BUTTON_REFRESH_LISTS, UI_BUTTON_REFRESH_LISTS);

	if(c.option(CONFIG_UI_TEXTFIELD_DEFAULT_WIDTH).empty())
		c.option(CONFIG_UI_TEXTFIELD_DEFAULT_WIDTH, UI_TEXTFIELD_DEFAULT_WIDTH);

	if(c.option(CONFIG_UI_TEXTFIELD_DEFAULT_HEIGHT).empty())
		c.option(CONFIG_UI_TEXTFIELD_DEFAULT_HEIGHT, UI_TEXTFIELD_DEFAULT_HEIGHT);

	
	if(c.option(CONFIG_UI_MOUSE_POINTER_IMAGE).empty())
		c.option(CONFIG_UI_MOUSE_POINTER_IMAGE, UI_MOUSE_POINTER_IMAGE);
	
	if(c.option(CONFIG_UI_TILEPALETTE_SHOW_IMAGE).empty())
		c.option(CONFIG_UI_TILEPALETTE_SHOW_IMAGE, UI_TILEPALETTE_SHOW_IMAGE);

	if(c.option(CONFIG_UI_TILEPALETTE_DOWN_IMAGE).empty())
		c.option(CONFIG_UI_TILEPALETTE_DOWN_IMAGE, UI_TILEPALETTE_DOWN_IMAGE);

	if(c.option(CONFIG_UI_TILEPALETTE_NEXT_IMAGE).empty())
		c.option(CONFIG_UI_TILEPALETTE_NEXT_IMAGE, UI_TILEPALETTE_NEXT_IMAGE);

	if(c.option(CONFIG_UI_TILEPALETTE_PREV_IMAGE).empty())
		c.option(CONFIG_UI_TILEPALETTE_PREV_IMAGE, UI_TILEPALETTE_PREV_IMAGE);

	if(c.option(CONFIG_UI_TILEPALETTE_BUCKET_IMAGE).empty())
		c.option(CONFIG_UI_TILEPALETTE_BUCKET_IMAGE, UI_TILEPALETTE_BUCKET_IMAGE);

	
	if(c.option(CONFIG_EDITOR_MAPS_PATH).empty())
		c.option(CONFIG_EDITOR_MAPS_PATH, EDITOR_MAPS_PATH);

	if(c.option(CONFIG_EDITOR_TILESETS_PATH).empty())
		c.option(CONFIG_EDITOR_TILESETS_PATH, EDITOR_TILESETS_PATH);

	if(c.option(CONFIG_EDITOR_NEW_MAP_NAME).empty())
		c.option(CONFIG_EDITOR_NEW_MAP_NAME, EDITOR_NEW_MAP_NAME);
}
