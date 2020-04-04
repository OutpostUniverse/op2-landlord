#pragma once

#include "NAS2D/NAS2D.h"

#include "EditorState.h"

// UI
#include "Button.h"
#include "ListBox.h"
#include "TextArea.h"
#include "TextField.h"


using namespace NAS2D;


/**
 * \class StartState
 * \brief Implements a startup state for the CoM Map Editor.
 */
class StartState: public State
{
public:

	StartState();
	~StartState();

protected:

	void initialize();

	State* update();

	void onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onDoubleClick(EventHandler::MouseButton button, int x, int y);
	void onWindowResize(int width, int height);

	void onQuit();

private:
	StringList getFileList(const std::string& directory);
		
	void fillMapMenu();
	
	void initUi();
	void updateUi();

	void resizeLayout();

	void button_CreateNew_click();
	void button_LoadExisting_click();
	void button_RefreshLists_click();

	void unsetSizeButtons();

	void btn64x64_Clicked();
	void btn64x128_Clicked();
	void btn64x256_Clicked();

	void btn128x128_Clicked();
	void btn128x64_Clicked();
	void btn128x256_Clicked();

	void btn256x256_Clicked();
	void btn256x128_Clicked();
	void btn512x256_Clicked();

	void btnMud_Clicked();
	void btnRock_Clicked();
	void btnSand_Clicked();

private:
	Timer			mTimer;					/**< used to flash messages. */

	Font			mFont;					/**< Internal Font to use. */
	Font			mBoldFont;				/**< Internal Font to use. */

	Point_2d		mMapSize;				/**< Rectangle used to position UI elements on screen. */
	Point_2d		mMouseCoords;			/**< Mouse pointer coordinates. */

	Rectangle_2d	mLayoutRect;			/**< Rectangle used to position UI elements on screen. */

	Button			mBtnCreateNew;			/**< Create New button. */
	Button			mBtnLoadExisting;		/**< Load Existing button. */
	Button			mBtnRefreshLists;		/**< Refresh File Lists button. */

	// Map Size Buttons
	Button			mBtn64x64;				/**<  */
	Button			mBtn64x128;				/**<  */
	Button			mBtn64x256;				/**<  */

	Button			mBtn128x128;			/**<  */
	Button			mBtn128x64;				/**<  */
	Button			mBtn128x256;			/**<  */

	Button			mBtn256x256;			/**<  */
	Button			mBtn256x128;			/**<  */
	Button			mBtn512x256;			/**<  */

	Button			mBtnMud;				/**<  */
	Button			mBtnRock;				/**<  */
	Button			mBtnSand;				/**<  */

	TextArea		txtMapDescription;		/**<  */

	TextField		txtMapPath;				/**< Filename for the map. */

	ListBox			mMapFilesMenu;			/**< Map File List menu. */

	bool			mScanningMaps = true;	/**<  */

	State*			mReturnState = this;	/**< State to return during updates. */
};
