#pragma once

#include "NAS2D/NAS2D.h"

#include "Menu.h"
#include "MiniMap.h"
#include "TilePalette.h"
#include "ToolBar.h"

#include "Map/Entity.h"
#include "Map/Map.h"

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

	void onKeyUp(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod);
	void onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onMouseUp(NAS2D::EventHandler::MouseButton button, int x, int y);
	void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);
	
	void onQuit();

private:

	EditorState();	// Explicitly undefined

	void fillTables();
	void fillEditStateStringTable();
	void fillStateToLayerTable();

	void initUi();

	void button_MapLinkOkay_Click();
	void button_MapLinkCancel_Click();
	
	void updateScroll();
	void updateSelector();

	void saveMap();

	void debug();
	void instructions();

	void changeTileTexture();
	void pattern(Cell::TileLayer layer, int value = 0);
	void patternFill(Cell::TileLayer layer);
	void patternFill_Contig(Cell::TileLayer layer, const Point_2d& _pt, int seed_index);

	void pattern_collision();

	void handleLeftButtonDown(int x, int y);

	void saveUndo();

	void setState(EditState state);
	void restorePreviousState();

	void updateUI();

	void toolbar_event(ToolBar::ToolBarAction _act);

private:

	// RESOURCES
	Timer			mTimer;

	Font			mFont;

	Image*			mMousePointer;

	Image			mPointer_Normal;
	Image			mPointer_Fill;
	Image			mPointer_Eraser;

	Image			mLayerHidden;

	// PRIMITIVES
	Point_2d		mMouseCoords;
	Point_2d		mSavedMouseCoords;
	Point_2df		mScrollVector;
	Rectangle_2d	mSelectorRect;
	Rectangle_2d	mCellInspectRect;

	// UI ELEMENTS
	TilePalette		mTilePalette;
	ToolBar			mToolBar;
	MiniMap			mMiniMap;

	Button			mBtnLinkOkay;
	Button			mBtnLinkCancel;

	TextField		mTxtLinkDestination;
	TextField		mTxtLinkDestX;
	TextField		mTxtLinkDestY;

	// MAP CONTROLS
	Cell*			mLinkCell;
	GameField		mFieldUndo;
	Map				mMap;

	std::string		mMapSavePath;

	EditState		mEditState;
	EditState		mPreviousEditState;

	// FLAGS
	bool			mDrawDebug;
	bool			mLeftButtonDown;
	bool			mRightButtonDown;
	bool			mPlacingCollision;		/**< Flag indicating whether or not to place or clear collision on mouse drags. */
	bool			mHideUi;				/**< Flag indicating that only the map be drawn. */
	bool			mMapChanged;			/**< Used to determine if the map changed. */

	State*			mReturnState;
};
