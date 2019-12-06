#include "StartState.h"


#include "Common.h"
#include "Defaults.h"
#include "Strings.h"


#include <iostream>

using namespace NAS2D;

std::string	MESSAGE = "";
std::string MAP_DESCRIPTION = "";


#ifdef WINDOWS
#include <Windows.h>
#endif


bool MSG_FLASH = false;
int NEW_MAP_PANEL_CENTER = 0;

BaseTerrain BASE_TERRAIN = TERRAIN_MUD;


void setMessage(const std::string& msg)
{
	MESSAGE = msg;
	std::cout << msg << std::endl;
}


/**
 * C'tpr
 */
StartState::StartState():	mFont("fonts/opensans.ttf", 12),
							mBoldFont("fonts/opensans-bold.ttf", 12),
							mMapSize(64, 64)
{}


StartState::~StartState()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.keyDown().disconnect(this, &StartState::onKeyDown);
	e.mouseMotion().disconnect(this, &StartState::onMouseMove);
	e.mouseDoubleClick().disconnect(this, &StartState::onDoubleClick);
	e.windowResized().disconnect(this, &StartState::onWindowResize);
	e.quit().disconnect(this, &StartState::onQuit);
}


/**
 * Called after the State is constructed.
 * 
 * Sets up initial values, hooks up event handlers and gets
 * the State ready for normal operation.
 */
void StartState::initialize()
{
	Configuration& c = Utility<Configuration>::get();

	mReturnState = this;

	setMessage("");

	mLayoutRect(15, 15, Utility<Renderer>::get().width() - 30, Utility<Renderer>::get().height() - 40);

	initUi();

	// Hook up event handlers
	EventHandler& e = Utility<EventHandler>::get();

	e.keyDown().connect(this, &StartState::onKeyDown);
	e.mouseMotion().connect(this, &StartState::onMouseMove);
	e.mouseDoubleClick().connect(this, &StartState::onDoubleClick);
	e.windowResized().connect(this, &StartState::onWindowResize);
	e.quit().connect(this, &StartState::onQuit);
}


void StartState::initUi()
{
	Renderer& r = Utility<Renderer>::get();

	txtMapDescription.font(mFont);
	txtMapDescription.text(MAP_64_X_64);

	// =========================================
	// = LOAD MAP PANEL
	// =========================================
	mBtnLoadExisting.font(mBoldFont);
	mBtnLoadExisting.size(85, 25);
	mBtnLoadExisting.text("Load Map");
	mBtnLoadExisting.click().connect(this, &StartState::button_LoadExisting_click);
	mBtnLoadExisting.enabled(false);

	mBtnRefreshLists.font(mBoldFont);
	mBtnRefreshLists.size(100, 25);
	mBtnRefreshLists.text("Refresh List");
	mBtnRefreshLists.click().connect(this, &StartState::button_RefreshLists_click);

	mMapFilesMenu.font(mBoldFont);


	// =========================================
	// = NEW MAP PANEL
	// =========================================
	// First Row
	mBtn64x64.font(mBoldFont);
	mBtn64x64.text("64x64");
	mBtn64x64.type(Button::BUTTON_TOGGLE);
	mBtn64x64.size(100, 20);
	mBtn64x64.toggle(true);
	mBtn64x64.click().connect(this, &StartState::btn64x64_Clicked);

	mBtn64x128.font(mBoldFont);
	mBtn64x128.text("64x128");
	mBtn64x128.type(Button::BUTTON_TOGGLE);
	mBtn64x128.size(100, 20);
	mBtn64x128.toggle(false);
	mBtn64x128.click().connect(this, &StartState::btn64x128_Clicked);

	mBtn64x256.font(mBoldFont);
	mBtn64x256.text("64x256");
	mBtn64x256.type(Button::BUTTON_TOGGLE);
	mBtn64x256.size(100, 20);
	mBtn64x256.toggle(false);
	mBtn64x256.click().connect(this, &StartState::btn64x256_Clicked);


	// Second Row
	mBtn128x128.font(mBoldFont);
	mBtn128x128.text("128x128");
	mBtn128x128.type(Button::BUTTON_TOGGLE);
	mBtn128x128.size(100, 20);
	mBtn128x128.toggle(false);
	mBtn128x128.click().connect(this, &StartState::btn128x128_Clicked);

	mBtn128x64.font(mBoldFont);
	mBtn128x64.text("128x64");
	mBtn128x64.type(Button::BUTTON_TOGGLE);
	mBtn128x64.size(100, 20);
	mBtn128x64.toggle(false);
	mBtn128x64.click().connect(this, &StartState::btn128x64_Clicked);

	mBtn128x256.font(mBoldFont);
	mBtn128x256.text("128x256");
	mBtn128x256.type(Button::BUTTON_TOGGLE);
	mBtn128x256.size(100, 20);
	mBtn128x256.toggle(false);
	mBtn128x256.click().connect(this, &StartState::btn128x256_Clicked);

	/// Third Row
	mBtn256x256.font(mBoldFont);
	mBtn256x256.text("256x256");
	mBtn256x256.type(Button::BUTTON_TOGGLE);
	mBtn256x256.size(100, 20);
	mBtn256x256.toggle(false);
	mBtn256x256.click().connect(this, &StartState::btn256x256_Clicked);

	mBtn256x128.font(mBoldFont);
	mBtn256x128.text("256x128");
	mBtn256x128.type(Button::BUTTON_TOGGLE);
	mBtn256x128.size(100, 20);
	mBtn256x128.toggle(false);
	mBtn256x128.click().connect(this, &StartState::btn256x128_Clicked);

	mBtn512x256.font(mBoldFont);
	mBtn512x256.text("512x256");
	mBtn512x256.type(Button::BUTTON_TOGGLE);
	mBtn512x256.size(100, 20);
	mBtn512x256.toggle(false);
	mBtn512x256.click().connect(this, &StartState::btn512x256_Clicked);

	mBtnCreateNew.font(mBoldFont);
	mBtnCreateNew.size(85, 25);
	mBtnCreateNew.text("Create New");
	mBtnCreateNew.click().connect(this, &StartState::button_CreateNew_click);

	// TERRAIN TYPES
	mBtnMud.image("sys/mud.png");
	mBtnMud.size(135);
	mBtnMud.type(Button::BUTTON_TOGGLE);
	mBtnMud.toggle(true);
	mBtnMud.click().connect(this, &StartState::btnMud_Clicked);

	mBtnRock.image("sys/rock.png");
	mBtnRock.size(135);
	mBtnRock.type(Button::BUTTON_TOGGLE);
	mBtnRock.toggle(false);
	mBtnRock.click().connect(this, &StartState::btnRock_Clicked);

	mBtnSand.image("sys/sand.png");
	mBtnSand.size(135);
	mBtnSand.type(Button::BUTTON_TOGGLE);
	mBtnSand.toggle(false);
	mBtnSand.click().connect(this, &StartState::btnSand_Clicked);

	txtMapPath.font(mBoldFont);
	txtMapPath.text("");
	txtMapPath.maxCharacters(40);

	resizeLayout();
}


void StartState::resizeLayout()
{
	Renderer& r = Utility<Renderer>::get();

	mLayoutRect(15, 15, Utility<Renderer>::get().width() - 30, Utility<Renderer>::get().height() - 40);

	// =========================================
	// = LOAD MAP PANEL
	// =========================================
	mBtnLoadExisting.position(mLayoutRect.x() + 10, mLayoutRect.y() + mLayoutRect.height() - 35);
	mBtnRefreshLists.position(mLayoutRect.x() + (mLayoutRect.width() / 3) - mBtnRefreshLists.rect().width() - 10, mLayoutRect.y() + mLayoutRect.height() - 35);

	mMapFilesMenu.position(mLayoutRect.x() + 10, mLayoutRect.y() + 10);
	mMapFilesMenu.size(mLayoutRect.width() / 3 - 20, mLayoutRect.height() - 30 - mBtnLoadExisting.height());


	// =========================================
	// = NEW MAP PANEL
	// =========================================

	int _left_x = mLayoutRect.x() + (mLayoutRect.width() / 3) + 10;
	NEW_MAP_PANEL_CENTER = mLayoutRect.x() + (mLayoutRect.width() / 3) + ((mLayoutRect.width() - (mLayoutRect.width() / 3)) / 2);

	mBtn64x64.position(_left_x, 25);
	mBtn64x128.position(NEW_MAP_PANEL_CENTER - 60, 25);
	mBtn64x256.position(r.width() - 125, 25);

	mBtn128x128.position(_left_x, 55);
	mBtn128x64.position(NEW_MAP_PANEL_CENTER - 60, 55);
	mBtn128x256.position(r.width() - 125, 55);

	mBtn256x256.position(_left_x, 85);
	mBtn256x128.position(NEW_MAP_PANEL_CENTER - 60, 85);
	mBtn512x256.position(r.width() - 125, 85);

	mBtnCreateNew.position(mLayoutRect.x() + mLayoutRect.width() - 95, mLayoutRect.y() + mLayoutRect.height() - 35);

	txtMapPath.width(mLayoutRect.width() - (mLayoutRect.width() / 3) - 20);
	txtMapPath.position(mLayoutRect.x() + mLayoutRect.width() / 3 + 10, mBtnCreateNew.positionY() - 30);

	txtMapDescription.size(mLayoutRect.width() - (mLayoutRect.width() / 3) - 20, 25);
	txtMapDescription.position(mBtn64x64.positionX(), 115);

	mBtnMud.position(_left_x, txtMapDescription.positionY() + txtMapDescription.height() + 10);
	mBtnRock.position(NEW_MAP_PANEL_CENTER - mBtnRock.width() / 2, txtMapDescription.positionY() + txtMapDescription.height() + 10);
	mBtnSand.position(r.width() - mBtnSand.width() - 25, txtMapDescription.positionY() + txtMapDescription.height() + 10);
}


void StartState::fillMapMenu()
{
	StringList lst = getFileList(EDITOR_MAPS_PATH);

	for (auto item : lst)
	{
		mMapFilesMenu.addItem(item);
	}

	mBtnLoadExisting.enabled(!mMapFilesMenu.empty());
}


/**
 * Gets a list of files, ignoring directories.
 * 
 * \param	directory	Path to scan for files.
 * 
 * \return	Returns a StringList.
 */
StringList StartState::getFileList(const std::string& directory)
{
	StringList fileList = Utility<Filesystem>::get().directoryList(directory);
	StringList returnList;

	Filesystem& f = Utility<Filesystem>::get();

	for (size_t i = 0; i < fileList.size(); i++)
	{
		if (!f.isDirectory(directory + fileList[i]))
		{
			returnList.push_back(fileList[i]);
		}
	}

	return returnList;
}


/**
 * Click handler for mBtnCreateNew.
 */
void StartState::button_CreateNew_click()
{
	int mapWidth = 0, mapHeight = 0;

	txtMapPath.highlight(false);

	if (txtMapPath.text().empty())
	{
		txtMapPath.highlight(true);
		setMessage("Must enter a file name to proceed.");
		return;
	}

	Configuration& c = Utility<Configuration>::get();

	std::string mapname = txtMapPath.text();
	// check for file extension
	if (mapname.length() > 4)
	{
		if (NAS2D::toLowercase(mapname.substr(mapname.length() - 4)) != ".map")
		{
			mapname += ".map";
		}
	}

	try
	{
		mReturnState = new EditorState(mapname, BASE_TERRAIN, 128, 128);
	}
	catch (std::exception& e)
	{
		MESSAGE = e.what();
		std::cout << e.what() << std::endl;
		#ifdef WINDOWS
		MessageBoxA(NULL, e.what(), "Application Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif
	}
}


/**
 * Click handler for mBtnLoadExisting.
 */
void StartState::button_LoadExisting_click()
{
	if (mMapFilesMenu.currentSelection() == NO_SELECTION) { return; }
	
	std::string mapPath = EDITOR_MAPS_PATH + mMapFilesMenu.selectionText();

	// In the event someone does something completely idiotic like deleting map files after the
	// editor has scanned the maps directory (or some other error occurs where the map file is
	// no longer available), ensure that we prevent failure.
	if(!Utility<Filesystem>::get().exists(mapPath))
	{
		setMessage("ERROR: Selected file could not be found.");
		return;
	}

	/// Doing it this way only so that the user can get feedback about what the app is doing.
	/// \todo	This would benefit by spinning loading maps into its own thread.
	Utility<Renderer>::get().drawText(mBoldFont, "LOADING MAP. PLEASE WAIT...", mLayoutRect.x(), 5, 255, 255, 0);
	Utility<Renderer>::get().update();

	try
	{
		mReturnState = new EditorState(mapPath);
	}
	catch (std::runtime_error& e)
	{
		setMessage("COULDN'T LOAD MAP: " + std::string(e.what()));
	}
}


void StartState::button_RefreshLists_click()
{
	mMapFilesMenu.dropAllItems();
	mScanningMaps = true;
}


/**
 * Updates logic, performs any necessary tasks, draws everything
 * to the screen and returns a pointer to an active state.
 */
State* StartState::update()
{
	Renderer& r = Utility<Renderer>::get();
	r.clearScreen(25, 25, 25);

	r.drawBox(mLayoutRect, 255, 255, 255);
	r.drawLine(mLayoutRect.x() + mLayoutRect.width() / 3, mLayoutRect.y(), mLayoutRect.x() + mLayoutRect.width() / 3, mLayoutRect.y() + mLayoutRect.height());

	updateUi();

	if (mTimer.accumulator() > 200)
	{
		MSG_FLASH = !MSG_FLASH;
		mTimer.reset();
	}

	if (!MESSAGE.empty() && MSG_FLASH)
	{
		r.drawText(mBoldFont, MESSAGE, 15, r.height() - 20, 255, 0, 0);
	}

	r.drawBoxFilled(NEW_MAP_PANEL_CENTER - mMapSize.x() / 2, (r.height() - 223) - mMapSize.y() / 2, mMapSize.x(), mMapSize.y(), 185, 185, 185);

	/// Doing it this way only so that the user can get feedback about what the app is doing.
	/// \todo	This would benefit by spinning loading maps into its own thread.
	if (mScanningMaps)
	{
		r.drawText(mBoldFont, "SCANNING MAPS. PLEASE WAIT...", mLayoutRect.x(), 0, 255, 255, 0);
		r.update();
		fillMapMenu();
		mScanningMaps = false;
	}

	return mReturnState;
}


void StartState::updateUi()
{
	// Load Map Panel
	mBtnCreateNew.update();
	mBtnLoadExisting.update();
	mBtnRefreshLists.update();

	txtMapDescription.update();
	txtMapPath.update();

	mMapFilesMenu.update();


	// New Map Panel
	mBtn64x64.update();
	mBtn64x128.update();
	mBtn64x256.update();

	mBtn128x128.update();
	mBtn128x64.update();
	mBtn128x256.update();

	mBtn256x256.update();
	mBtn256x128.update();
	mBtn512x256.update();

	mBtnMud.update();
	mBtnRock.update();
	mBtnSand.update();
}


/**
 * Key Down handler.
 */
void StartState::onKeyDown(EventHandler::KeyCode key, EventHandler::KeyModifier mod, bool repeat)
{
	if (key == EventHandler::KEY_ESCAPE)
	{
		mReturnState = nullptr;
	}
}


/**
 * Mouse Motion handler.
 */
void StartState::onMouseMove(int x, int y, int relX, int relY)
{
	mMouseCoords(x, y);
}


void StartState::onDoubleClick(EventHandler::MouseButton button, int x, int y)
{
	if (button != EventHandler::BUTTON_LEFT) { return; }

	if (isPointInRect(mMouseCoords, mMapFilesMenu.rect()))
	{
		button_LoadExisting_click();
	}
}


void StartState::onWindowResize(int width, int height)
{
	resizeLayout();
}


/**
 * Quit handler.
 */
void StartState::onQuit()
{
	mReturnState = NULL;
}


void StartState::unsetSizeButtons()
{
	mBtn64x64.toggle(false);
	mBtn64x128.toggle(false);
	mBtn64x256.toggle(false);

	mBtn128x128.toggle(false);
	mBtn128x64.toggle(false);
	mBtn128x256.toggle(false);

	mBtn256x256.toggle(false);
	mBtn256x128.toggle(false);
	mBtn512x256.toggle(false);
}


void StartState::btn64x64_Clicked()
{
	unsetSizeButtons();
	mBtn64x64.toggle(true);
	mMapSize(64, 64);
	txtMapDescription.text(MAP_64_X_64);
}


void StartState::btn64x128_Clicked()
{
	unsetSizeButtons();
	mBtn64x128.toggle(true);
	mMapSize(64, 128);
	txtMapDescription.text(MAP_64_X_128);
}


void StartState::btn64x256_Clicked()
{
	unsetSizeButtons();
	mBtn64x256.toggle(true);
	mMapSize(64, 256);
	txtMapDescription.text(MAP_64_X_256);
}


void StartState::btn128x128_Clicked()
{
	unsetSizeButtons();
	mBtn128x128.toggle(true);
	mMapSize(128, 128);
	txtMapDescription.text(MAP_128_X_128);
}


void StartState::btn128x64_Clicked()
{
	unsetSizeButtons();
	mBtn128x64.toggle(true);
	mMapSize(128, 64);
	txtMapDescription.text(MAP_128_X_64);
}


void StartState::btn128x256_Clicked()
{
	unsetSizeButtons();
	mBtn128x256.toggle(true);
	mMapSize(128, 256);
	txtMapDescription.text(MAP_128_X_256);
}


void StartState::btn256x256_Clicked()
{
	unsetSizeButtons();
	mBtn256x256.toggle(true);
	mMapSize(256, 256);
	txtMapDescription.text(MAP_256_X_256);
}


void StartState::btn256x128_Clicked()
{
	unsetSizeButtons();
	mBtn256x128.toggle(true);
	mMapSize(256, 128);
	txtMapDescription.text(MAP_256_X_128);
}


void StartState::btn512x256_Clicked()
{
	unsetSizeButtons();
	mBtn512x256.toggle(true);
	mMapSize(512, 256);
	txtMapDescription.text(MAP_512_X_256);
}


void StartState::btnMud_Clicked()
{
	mBtnMud.toggle(true);
	mBtnRock.toggle(false);
	mBtnSand.toggle(false);
	BASE_TERRAIN = TERRAIN_MUD;
}


void StartState::btnRock_Clicked()
{
	mBtnMud.toggle(false);
	mBtnRock.toggle(true);
	mBtnSand.toggle(false);
	BASE_TERRAIN = TERRAIN_ROCK;
}


void StartState::btnSand_Clicked()
{
	mBtnMud.toggle(false);
	mBtnRock.toggle(false);
	mBtnSand.toggle(true);
	BASE_TERRAIN = TERRAIN_SAND;
}
