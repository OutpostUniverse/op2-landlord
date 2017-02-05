#ifndef __EDITOR_STATE__
#define __EDITOR_STATE__

#include "NAS2D/NAS2D.h"

#include "Entity.h"
#include "Map.h"
#include "Menu.h"
#include "TilePalette.h"
#include "ToolBar.h"

#include "Button.h"
#include "TextField.h"

#include <string>
#include <map>

using namespace std;
using namespace NAS2D;

enum EditState
{
	STATE_BASE_TILE_INDEX,
	STATE_BASE_DETAIL_TILE_INDEX,
	STATE_DETAIL_TILE_INDEX,
	STATE_FOREGROUND_TILE_INDEX,
	STATE_TILE_COLLISION,
	STATE_TILE_ERASE,
	STATE_MAP_LINK_EDIT
};


enum HandleCorner
{
	HANDLE_NONE = 0,
	HANDLE_TOP_LEFT,
	HANDLE_TOP_RIGHT,
	HANDLE_BOTTOM_LEFT,
	HANDLE_BOTTOM_RIGHT
};



class EditorState: public State
{
public:
	EditorState(const std::string& mapPath);
	EditorState(const std::string& name, const std::string& mapPath, const std::string& tsetPath, int width, int height);

	~EditorState();

protected:
	void initialize();
	State* update();

	void onKeyUp(KeyCode key, KeyModifier mod);
	void onKeyDown(KeyCode key, KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onMouseUp(MouseButton button, int x, int y);
	void onMouseDown(MouseButton button, int x, int y);
	
	void onQuit();

private:

	EditorState();	// Explicitly undefined

	void fillEditStateStringTable();

	void initUi();


	// FUNCTIONS
	// ===============================
	void button_MapLinkOkay_Click();
	void button_MapLinkCancel_Click();
	
	void updateScroll();
	void updateSelector();

	void saveMap();

	void debug();
	void instructions();

	void changeTileTexture();
	void patternFill(Cell::TileLayer layer);
	void patternStamp(Cell::TileLayer layer);

	void showTilePalette();
	void hideTilePalette();

	void handleLeftButtonDown(int x, int y);
	void handleRightButtonDown();
	void handleMiddleButtonDown();

	void saveUndo();

	void setState(EditState state);
	void restorePreviousState();

	void updateUI();
	void drawMiniMap();
	void createMiniMap();


	// MEMBERS
	// ===============================
	Timer			mTimer;

	Rectangle_2d	mSelectorRect;

	Point_2d		mMouseCoords;
	Image			mMousePointer;

	TilePalette		mTilePalette;

	Point_2df		mScrollVector;

	Font			mFont;
	
	ToolBar			mToolBar;

	Button			mBtnLinkOkay;
	Button			mBtnLinkCancel;

	TextField		mTxtLinkDestination;
	TextField		mTxtLinkDestX;
	TextField		mTxtLinkDestY;

	Rectangle_2d	mCellInspectRect;

	Cell*			mLinkCell;

	Map				mMap;

	GameField		mFieldUndo;

	std::string		mMapSavePath;

	Image*			mMiniMap;

	EditState		mEditState;
	EditState		mPreviousEditState;

	bool			mDrawDebug;

	bool			mLeftButtonDown;
	bool			mRightButtonDown;

	bool			mPlacingCollision;		/**< Flag indicating whether or not to place or clear collision on mouse drags. */

	bool			mHideUi;				/**< Flag indicating that only the map be drawn. */
	bool			mDrawMiniMap;			/**< Flag indicating that we should draw the minimap. */
	bool			mMapChanged;			/**< Used to determine if the map changed. */

	State*			mReturnState;
};


#endif