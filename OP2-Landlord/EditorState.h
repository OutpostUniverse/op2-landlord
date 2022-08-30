#pragma once

#include "NAS2D/NAS2D.h"

#include "Fonts.h"
#include "MiniMap.h"
#include "TileGroups.h"
#include "TilePalette.h"
#include "ToolBar.h"

#include "Map/MapFile.h"

#include <string>
#include <map>


class EditorState: public State
{
public:
	EditorState(const std::string& mapPath);
	EditorState(const std::string& mapPath, BaseTerrain base, int width, int height);
	virtual ~EditorState();

protected:
	/** Inherited functions. */
	virtual void initialize() final;
	virtual State* update() final;

	/** Event Handlers */
	void onKeyUp(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod);
	void onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat);
	void onMouseMove(NAS2D::Point<int> position, NAS2D::Vector<int> change);
	void onMouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position);
	void onMouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position);
	void onWindowResized(NAS2D::Vector<int> newSize);
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
	void patternFill_Contig(const NAS2D::Point<int>& _pt, int seed_index);

	void pattern_collision();
	void handleLeftButtonDown(int x, int y);

	void saveUndo();
	void updateUI();

	void toolbar_event(ToolBar::ToolBarAction _act);

private:
	NAS2D::Timer			mTimer;
	const NAS2D::Font& mFont = fontCache.load("fonts/opensans.ttf", 12);
	const NAS2D::Font& mBoldFont = fontCache.load("fonts/opensans-bold.ttf", 12);

	MapFile*		mMap = nullptr;				/**< Map object. */

	std::string		mMapSavePath;				/**< Filename to use for loading/saving. */

	// PRIMITIVES
	NAS2D::Point<int>		mMouseCoords;
	NAS2D::Point<int>		mSavedMouseCoords;
	NAS2D::Point<int>		mTileHighlight;

	NAS2D::Rectangle<int>	mSelectorRect;
	NAS2D::Rectangle<int>	mCellInspectRect;

	// UI ELEMENTS
	TileGroups		mTileGroups;
	TilePalette		mTilePalette;
	ToolBar			mToolBar;
	MiniMap			mMiniMap;

	// FLAGS
	bool			mLeftButtonDown = false;	/**< Flag for left mouse button down. */
	bool			mRightButtonDown = false;	/**< Flag for right mouse button down. */
	bool			mMapChanged = false;		/**< Used to determine if the map changed. */

	NAS2D::State*			mReturnState = this;		/**< Return state (nullptr terminates the program). */
};
