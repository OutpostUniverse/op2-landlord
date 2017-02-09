#include "StartState.h"

#include "Defaults.h"

#include "Common.h"

const int LAYOUT_RECT_WIDTH			= 790;
const int LAYOUT_RECT_HEIGHT		= 590;

std::string	MESSAGE = "";

bool	MSG_FLASH = false;

void setMessage(const std::string& msg)
{
	MESSAGE = msg;
	cout << msg << endl;
}


/**
 * C'tpr
 */
StartState::StartState():	mFont("fonts/ui-normal.png", 7, 9, 0),
							mMousePointer("sys/normal.png"),
							mLayoutRect(15, 15, Utility<Renderer>::get().width() - 30, Utility<Renderer>::get().height() - 40),
							mReturnState(nullptr)
{

}


StartState::~StartState()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.keyDown().Disconnect(this, &StartState::onKeyDown);
	e.mouseMotion().Disconnect(this, &StartState::onMouseMove);
	e.quit().Disconnect(this, &StartState::onQuit);
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

	mBtnCreateNew.font(mFont);
	mBtnCreateNew.size(85, 25);
	mBtnCreateNew.text("Create New");
	mBtnCreateNew.position(mLayoutRect.x() + mLayoutRect.w() - 95, mLayoutRect.y() + mLayoutRect.h() - 35);
	mBtnCreateNew.click().Connect(this, &StartState::button_CreateNew_click);

	mBtnLoadExisting.font(mFont);
	mBtnLoadExisting.size(85, 25);
	mBtnLoadExisting.text("Load Map");
	mBtnLoadExisting.position(mLayoutRect.x() + 10, mLayoutRect.y() + mLayoutRect.h() - 35);
	mBtnLoadExisting.click().Connect(this, &StartState::button_LoadExisting_click);

	mBtnRefreshLists.font(mFont);
	mBtnRefreshLists.size(100, 25);
	mBtnRefreshLists.text("Refresh List");
	mBtnRefreshLists.position(mLayoutRect.x() + (mLayoutRect.w() / 2) - mBtnRefreshLists.rect().w() - 10, mLayoutRect.y() + mLayoutRect.h() - 35);
	mBtnRefreshLists.click().Connect(this, &StartState::button_RefreshLists_click);


	mTxtWidth.font(mFont);
	mTxtWidth.width(100);
	mTxtWidth.text(UI_TEXTFIELD_DEFAULT_WIDTH);
	mTxtWidth.position(mLayoutRect.x() + mLayoutRect.w() / 2 + 10 + mFont.width("Width:") + 5, mLayoutRect.y() + 10);
	mTxtWidth.border(TextField::ALWAYS);

	mTxtHeight.font(mFont);
	mTxtHeight.width(100);
	mTxtHeight.text(c.option(UI_TEXTFIELD_DEFAULT_HEIGHT));
	mTxtHeight.position(mLayoutRect.x() + mLayoutRect.w() / 2 + 210 + mFont.width("Height:") + 5, mLayoutRect.y() + 10);
	mTxtHeight.border(TextField::ALWAYS);

	txtMapPath.font(mFont);
	txtMapPath.width(mLayoutRect.x() + mLayoutRect.w() / 2 - 40);
	txtMapPath.text("");
	txtMapPath.position(mLayoutRect.x() + mLayoutRect.w() / 2 + 10, mBtnCreateNew.positionY() - 30);

	mMapFilesMenu.font(mFont);
	mMapFilesMenu.position(mLayoutRect.x() + 10, mLayoutRect.y() + 10);
	scanDirectory(EDITOR_MAPS_PATH, mMapFilesMenu);
	mMapFilesMenu.width(mLayoutRect.w() / 2 - 20);

	mTsetFilesMenu.font(mFont);
	mTsetFilesMenu.position(mLayoutRect.x() + mLayoutRect.w() / 2 + 10, mLayoutRect.y() + 50);
	scanDirectory(EDITOR_TSET_PATH, mTsetFilesMenu);
	mTsetFilesMenu.width(mLayoutRect.w() / 2 - 20);


	// Hook up event handlers
	EventHandler& e = Utility<EventHandler>::get();

	e.keyDown().Connect(this, &StartState::onKeyDown);
	e.mouseMotion().Connect(this, &StartState::onMouseMove);
	e.quit().Connect(this, &StartState::onQuit);
}


/**
 * Scans a directory for files, ignoring directories, and
 * adds them to a Menu.
 * 
 * \param	directory	Path to scan for files.
 * \param	menu		Reference to a Menu object to add file listings to.
 */
void StartState::scanDirectory(const string& directory, Menu& menu)
{
	StringList fileList = Utility<Filesystem>::get().directoryList(directory);

	for(size_t i = 0; i < fileList.size(); i++)
	{
		if(!Utility<Filesystem>::get().isDirectory(directory + fileList[i]))
			menu.addItem(fileList[i]);
	}
}


/**
 * Click handler for mBtnCreateNew.
 */
void StartState::button_CreateNew_click()
{
	int mapWidth = 0, mapHeight = 0;

	mTxtWidth.highlight(false);
	mTxtHeight.highlight(false);
	txtMapPath.highlight(false);

	if(!from_string<int>(mapWidth, mTxtWidth.text(), std::dec))
	{
	}

	if(!from_string<int>(mapHeight, mTxtHeight.text(), std::dec))
	{
		mTxtHeight.highlight(true);
		setMessage("Map width field must be an integer.");
		return;
	}

	// sanity check for map widths
	if (mapWidth < 10)
	{
		mTxtWidth.highlight(true);
		setMessage("Map width must be at least 10.");
		return;
	}

	// sanity check for map widths
	if (mapHeight < 10)
	{
		mTxtHeight.highlight(true);
		setMessage("Map height must be at least 10.");
		return;
	}


	if (txtMapPath.text().empty())
	{
		txtMapPath.highlight(true);
		setMessage("Must enter a file name to proceed.");
		return;
	}

	Configuration& c = Utility<Configuration>::get();
	mReturnState = new EditorState(EDITOR_NEW_MAP_NAME, EDITOR_MAPS_PATH + txtMapPath.text(), EDITOR_TSET_PATH + mTsetFilesMenu.selectionText(), mapWidth, mapHeight);
}


/**
 * Click handler for mBtnLoadExisting.
 */
void StartState::button_LoadExisting_click()
{
	string mapPath = EDITOR_MAPS_PATH + mMapFilesMenu.selectionText();

	// In the event someone does something completely idiotic like deleting map files after the
	// editor has scanned the maps directory (or some other error occurs where the map file is
	// no longer available), ensure that we prevent failure.
	if(!Utility<Filesystem>::get().exists(mapPath))
	{
		setMessage("ERROR: Selected file could not be found.");
		return;
	}

	mReturnState = new EditorState(mapPath);
}


void StartState::button_RefreshLists_click()
{
	mMapFilesMenu.dropAllItems();
	mTsetFilesMenu.dropAllItems();

	Configuration& c = Utility<Configuration>::get();

	scanDirectory(EDITOR_MAPS_PATH, mMapFilesMenu);
	scanDirectory(EDITOR_TSET_PATH, mTsetFilesMenu);
}


/**
 * Updates logic, performs any necessary tasks, draws everything
 * to the screen and returns a pointer to an active state.
 */
State* StartState::update()
{
	Renderer& r = Utility<Renderer>::get();
	r.clearScreen(COLOR_BLACK);

	r.drawBox(mLayoutRect, 255, 255, 255);
	r.drawLine(mLayoutRect.x() + mLayoutRect.w() / 2, mLayoutRect.y(), mLayoutRect.x() + mLayoutRect.w() / 2, mLayoutRect.y() + mLayoutRect.h());

	mBtnCreateNew.update();
	mBtnLoadExisting.update();
	mBtnRefreshLists.update();

	r.drawText(mFont, "Width:", mLayoutRect.x() + mLayoutRect.w() / 2 + 10, mLayoutRect.y() + 15, 255, 255, 255);
	r.drawText(mFont, "Height:", mLayoutRect.x() + mLayoutRect.w() / 2 + 210, mLayoutRect.y() + 15, 255, 255, 255);

	mTxtWidth.update();
	mTxtHeight.update();
	txtMapPath.update();

	mMapFilesMenu.update();
	mTsetFilesMenu.update();

	if (mTimer.accumulator() > 200)
	{
		MSG_FLASH = !MSG_FLASH;
		mTimer.reset();
	}

	if (!MESSAGE.empty() && MSG_FLASH)
		r.drawText(mFont, MESSAGE, 15, r.height() - 15, 255, 0, 0);

	r.drawImage(mMousePointer, mMouseCoords.x(), mMouseCoords.y());

	return mReturnState;
}




/**
 * Key Down handler.
 */
void StartState::onKeyDown(KeyCode key, KeyModifier mod, bool repeat)
{
	if(key == KEY_ESCAPE)
		mReturnState = NULL;
}


/**
 * Mouse Motion handler.
 */
void StartState::onMouseMove(int x, int y, int relX, int relY)
{
	mMouseCoords(x, y);
}


/**
 * Quit handler.
 */
void StartState::onQuit()
{
	mReturnState = NULL;
}

