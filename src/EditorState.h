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
	virtual ~EditorState();

protected:
	/** Inherited functions. */
	virtual void initialize() final;
	virtual State* update() final;

	/** Event Handlers */
	void onKeyUp(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod);
	void onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onMouseUp(NAS2D::EventHandler::MouseButton button, int x, int y);
	void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);
	void onWindowResized(int x, int y);
	void onQuit();

private:
	EditorState() = delete;									// Not allowed.
	EditorState(const EditorState&) = delete;				// Not allowed.
	EditorState& operator=(const EditorState&) = delete;	// Not allowed.

	void initUi();
	void updateSelector();
	void saveMap();
	void debug();

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
	Timer			mTimer;						/**<  */
	Font			mFont;						/**<  */
	Font			mBoldFont;					/**<  */

	MapFile*		mMap = nullptr;				/**< Map object. */

	std::string		mMapSavePath;				/**< Filename to use for loading/saving. */

	// PRIMITIVES
	Point_2d		mMouseCoords;				/**<  */
	Point_2d		mSavedMouseCoords;			/**<  */
	Point_2d		mTileHighlight;				/**<  */

	Rectangle_2d	mSelectorRect;				/**<  */
	Rectangle_2d	mCellInspectRect;			/**<  */

	// UI ELEMENTS
	TilePalette		mTilePalette;				/**<  */
	ToolBar			mToolBar;					/**<  */
	MiniMap			mMiniMap;					/**<  */

	// FLAGS
	bool			mLeftButtonDown = false;	/**< Flag for left mouse button down. */
	bool			mRightButtonDown = false;	/**< Flag for right mouse button down. */
	bool			mPlacingCollision = false;	/**< Flag indicating whether or not to place or clear collision on mouse drags. */
	bool			mMapChanged = false;		/**< Used to determine if the map changed. */

	State*			mReturnState = this;		/**< Return state (nullptr terminates the program). */
};
