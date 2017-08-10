#pragma once

#include "NAS2D/NAS2D.h"

#include "Menu.h"
#include "MiniMap.h"
#include "TilePalette.h"
#include "ToolBar.h"

#include "Map/MapFile.h"

#include <string>
#include <map>

using namespace std;
using namespace NAS2D;


class EditorState: public State
{
public:
	EditorState(const std::string& mapPath);
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

	void initUi();

	void button_MapLinkOkay_Click();
	void button_MapLinkCancel_Click();
	
	void updateScroll();
	void updateSelector();

	void saveMap();

	void debug();
	void instructions();

	void changeTileTexture();
	void pattern(int value = 0);
	void patternFill();
	void patternFill_Contig(const Point_2d& _pt, int seed_index);

	void pattern_collision();

	void handleLeftButtonDown(int x, int y);

	void saveUndo();

	void updateUI();

	void toolbar_event(ToolBar::ToolBarAction _act);

private:
	Timer			mTimer;
	Font			mFont;
	Font			mBoldFont;

	MapFile*		mMapFile = nullptr;

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

	std::string		mMapSavePath;

	// FLAGS
	bool			mDrawDebug = false;
	bool			mLeftButtonDown = false;
	bool			mRightButtonDown = false;
	bool			mPlacingCollision = false;	/**< Flag indicating whether or not to place or clear collision on mouse drags. */
	bool			mHideUi = false;				/**< Flag indicating that only the map be drawn. */
	bool			mMapChanged = false;		/**< Used to determine if the map changed. */

	State*			mReturnState = nullptr;
};
