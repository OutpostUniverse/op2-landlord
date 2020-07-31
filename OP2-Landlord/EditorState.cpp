#include "EditorState.h"
#include "StartState.h"

#include "Common.h"

#include "Map/cell_types.h"

#include <algorithm>
#include <stack>
#include <sstream>

using namespace NAS2D;

std::stack<Point<int>> FLOOD_STACK;					/**< Stack used for contiguous flood fill. */

/**
 * C'tor
 * 
 * This version of the constructor is used to load an existing map file.
 */
EditorState::EditorState(const std::string& mapPath) :
	mMapSavePath(mapPath)
{}


/**
 * C'tor
 * 
 * This version of the constructor is used to generate a new map file.
 */
EditorState::EditorState(const std::string& mapPath, BaseTerrain base, int width, int height)
{
	throw std::runtime_error("Not supported yet.");

	/**
	 * \todo	A map file must be generated here before the call to
	 *			initialize() is made.
	 */
}


/**
 * D'tor
 */
EditorState::~EditorState()
{
	Utility<EventHandler>::get().keyUp().disconnect(this, &EditorState::onKeyUp);
	Utility<EventHandler>::get().keyDown().disconnect(this, &EditorState::onKeyDown);
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &EditorState::onMouseMove);
	Utility<EventHandler>::get().mouseButtonUp().disconnect(this, &EditorState::onMouseUp);
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &EditorState::onMouseDown);
	Utility<EventHandler>::get().quit().disconnect(this, &EditorState::onQuit);
	Utility<EventHandler>::get().windowResized().disconnect(this, &EditorState::onWindowResized);
}


/**
 * Initializes internal values and sets up event handlers.
 */
void EditorState::initialize()
{
	// Hook up event handlers
	Utility<EventHandler>::get().keyUp().connect(this, &EditorState::onKeyUp);
	Utility<EventHandler>::get().keyDown().connect(this, &EditorState::onKeyDown);
	Utility<EventHandler>::get().mouseMotion().connect(this, &EditorState::onMouseMove);
	Utility<EventHandler>::get().mouseButtonUp().connect(this, &EditorState::onMouseUp);
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &EditorState::onMouseDown);
	Utility<EventHandler>::get().quit().connect(this, &EditorState::onQuit);
	Utility<EventHandler>::get().windowResized().connect(this, &EditorState::onWindowResized);

	mSelectorRect = {0, 0, TILE_SIZE, TILE_SIZE};

	initUi();

	Renderer& r = Utility<Renderer>::get();

	mMap = new MapFile(mMapSavePath);
	mMap->updateCameraAnchorArea(r.size().x, r.size().y - mToolBar.height());

	mTileGroups.map(mMap);
	mMiniMap.map(mMap);
}


void EditorState::initUi()
{
	// ToolBar
	mToolBar.toolbar_event().connect(this, &EditorState::toolbar_event);

	mTileGroups.font(mFont);
	mTileGroups.titleFont(mBoldFont);
	mTileGroups.visible(mToolBar.show_tilegroups());

	// Tile Palette
	mTilePalette.font(mFont);
	mTilePalette.titleFont(mBoldFont);
	mTilePalette.visible(mToolBar.show_tilepalette());

	// Mini Map
	mMiniMap.font(mFont);
	mMiniMap.titleFont(mBoldFont);
	mMiniMap.visible(mToolBar.show_minimap());
}


/**
 * 
 */
State* EditorState::update()
{
	Renderer& r = Utility<Renderer>::get();
	r.clearScreen(Color::Magenta);

	mMap->draw(0, 32, r.size().x, r.size().y - mToolBar.height(), mToolBar.show_collision_mask());

	r.drawTextShadow(mBoldFont, "Cell Type: " + getCellTypeString((CellType)mMap->cellType(mTileHighlight.x, mTileHighlight.y)), {5, 50}, {1, 1}, NAS2D::Color::White, NAS2D::Color::Black);

	updateSelector();
	updateUI();

	const auto text = "World Tile: " + std::to_string(mTileHighlight.x) + ", " + std::to_string(mTileHighlight.y);
	r.drawTextShadow(mBoldFont, text, NAS2D::Point{5, r.size().y - 30}, {1, 1}, NAS2D::Color::White, NAS2D::Color::Black);
	r.drawTextShadow(mBoldFont, "Map File: " + mMapSavePath, NAS2D::Point{5, r.size().y - mBoldFont.height()}, {1, 1}, NAS2D::Color::White, NAS2D::Color::Black);

	return mReturnState;
}


void EditorState::updateUI()
{
	mToolBar.update();

	mTileGroups.update();
	mTilePalette.update();
	mMiniMap.update();
}


/**
 * Draws the tile selector based on the pattern selected in the TilePalette.
 */
void EditorState::updateSelector()
{
	// Don't draw selector if the UI is hidden.
	if (mMouseCoords.y < 32 || mRightButtonDown) { return; }

	if (mTilePalette.responding_to_events() || mMiniMap.responding_to_events()) { return; }

	Renderer& r = Utility<Renderer>::get();

	int rectOffsetX = (mMap->cameraPosition().x % TILE_SIZE);
	int rectOffsetY = (mMap->cameraPosition().y % TILE_SIZE) + TILE_SIZE; // offset for the toolbar

	int gridX = gridLocation(mMouseCoords.x, mMap->cameraPosition().x, r.size().x);
	int gridY = gridLocation(mMouseCoords.y, mMap->cameraPosition().y + TILE_SIZE, r.size().y - TILE_SIZE);

	mSelectorRect = {gridX * TILE_SIZE - rectOffsetX, gridY * TILE_SIZE - rectOffsetY, TILE_SIZE, TILE_SIZE};

	// Draw Tile Selector
	int offsetX = 0, offsetY = 0;
	
	const Pattern& p = mToolBar.brush();

	for(int row = p.height(); row > 0; row--)
	{
		for(int col = p.width(); col > 0; col--)
		{
			r.drawBox(NAS2D::Rectangle{mSelectorRect.x - offsetX, mSelectorRect.y - offsetY + TILE_SIZE, mSelectorRect.width, mSelectorRect.height});
			offsetX += 32;
		}
		offsetX = 0;
		offsetY += 32;
	}
}


/**
 * Handles KeyDown events.
 */
void EditorState::onKeyDown(EventHandler::KeyCode key, EventHandler::KeyModifier mod, bool repeat)
{
	if (repeat) { return; }

	switch(key)
	{
		case EventHandler::KeyCode::KEY_ESCAPE:
			mReturnState = new StartState();
			break;

		case EventHandler::KeyCode::KEY_z:
			if(Utility<EventHandler>::get().control(mod))
			{
			}

			break;

		default:
			break;
	}
}


/**
 * Handles KeyUp events.
 */
void EditorState::onKeyUp(EventHandler::KeyCode key, EventHandler::KeyModifier mod)
{}


/**
 *
 */
void EditorState::onQuit()
{
	mReturnState = NULL;
}


/**
 * Window resized event handler.
 */
void EditorState::onWindowResized(int x, int y)
{
	// Need to account for the toolbar height for map drawing calculations to be correct.
	mMap->updateCameraAnchorArea(x, y - mToolBar.height());
}


/**
 *
 */
void EditorState::onMouseMove(int x, int y, int relX, int relY)
{
	if(mRightButtonDown)
	{
		mMap->moveCamera(-relX, -relY);
		return;
	}

	mMouseCoords = {x, y};

	mTileHighlight = {
		std::clamp((x + mMap->cameraPosition().x) / TILE_SIZE, 0, mMap->width() - 1),
		std::clamp((y + mMap->cameraPosition().y - TILE_SIZE) / TILE_SIZE, 0, mMap->height() - 1)
	};

	if(mLeftButtonDown)
	{
		// Avoid modifying tiles if we're in the 'toolbar area'
		if (y < 32 || mToolBar.flood() || mTilePalette.responding_to_events() || mMiniMap.responding_to_events())
			return;

		changeTileTexture();
	}
}


/**
 * Handles MouseDown events.
 */
void EditorState::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	Utility<EventHandler>::get().grabMouse();

	// Left Mouse Button
	if(button == EventHandler::MouseButton::BUTTON_LEFT)
	{
		mLeftButtonDown = true;
		handleLeftButtonDown(x, y);
	}
	else if(button == EventHandler::MouseButton::BUTTON_RIGHT)
	{
		mRightButtonDown = true;
		mSavedMouseCoords = mMouseCoords;
		Utility<EventHandler>::get().mouseRelativeMode(true);
	}
}


/**
 * Handles MouseUp events
 */
void EditorState::onMouseUp(EventHandler::MouseButton button, int x, int y)
{
	if(button == EventHandler::MouseButton::BUTTON_LEFT)
	{
		mLeftButtonDown = false;
		//if(mEditState == STATE_BASE_TILE_INDEX || mEditState == STATE_BASE_DETAIL_TILE_INDEX || mEditState == STATE_DETAIL_TILE_INDEX || mEditState == STATE_FOREGROUND_TILE_INDEX)
		//{
			if(mMapChanged)
			{
				mMiniMap.update_minimap();
				mMapChanged = false;
			}
		//}
	}
	else if(button == EventHandler::MouseButton::BUTTON_RIGHT)
	{
		mRightButtonDown = false;
		Utility<EventHandler>::get().mouseRelativeMode(false);
		Utility<EventHandler>::get().warpMouse(mSavedMouseCoords.x, mSavedMouseCoords.y); // a bit hacky but does the job
	}

	Utility<EventHandler>::get().releaseMouse();
}


/**
 * Handles left mouse button down.
 */
void EditorState::handleLeftButtonDown(int x, int y)
{
	Point<int> pt{x, y};

	// Hate the look of this but it effectively condenses the ignore checks.
	if (y < 32 ||
		(mToolBar.flood() && mToolBar.flood_tool_extended_area().contains(pt)) ||
		mTilePalette.rect().contains(pt) ||
		mMiniMap.rect().contains(pt) ||
		mTilePalette.rect().contains(pt))
		return;


	//Cell& cell = mMap.getCell(mMouseCoords);

	saveUndo();
	changeTileTexture();
}


/**
 * Changes the tile texture index of the highlighted Cell.
 * 
 * \note	Will not modify tile texture index unless in the correct edit mode.
 */
void EditorState::changeTileTexture()
{
	/*
	if (mToolBar.flood())
	{
		if (mToolBar.flood_contiguous())
			patternFill_Contig(StateToLayer[mEditState], mMap.getGridCoords(mMouseCoords), mMap.getCell(mMouseCoords).index(StateToLayer[mEditState]));
		else
			patternFill(StateToLayer[mEditState]);
	}
	else if (mToolBar.pencil())
		pattern(StateToLayer[mEditState]);
	else if (mToolBar.erase())
		pattern(StateToLayer[mEditState], -1);
	else // Defined this way to avoid forgetting to add possible new tools to the check.
		return;

	mMapChanged = true;
	*/
}


/**
 * Fills a given cell layer with a pattern.
 */
void EditorState::patternFill()
{
	/*
	const Pattern& p = mTilePalette.pattern();

	for (int row = 0; row < mMap.height; row++)
	{
		for (int col = 0; col < mMap.width; col++)
		{
			mMap.getCellByGridCoords(col, row).index(layer, p.value(col % p.width, row % p.height));
		}
	}
	*/
}


/**
 * Fills a contiguous area in a given layer with a pattern.
 */
void EditorState::patternFill_Contig(const Point<int>& _pt, int seed_index)
{
	/*
	const Pattern& _pCheck = mTilePalette.pattern();
	if (seed_index == _pCheck.value(_pt.x % _pCheck.width, _pt.y % _pCheck.height))
		return;

	while (!FLOOD_STACK.empty())
	{
		FLOOD_STACK.pop();
	}

	static const vector<int> dX = { 0, 1, 0, -1 }; // Neighbor Coords
	static const vector<int> dY = { -1, 0, 1, 0 }; // Neighbor Coords

	FLOOD_STACK.push(_pt);
	
	while(!FLOOD_STACK.empty())
	{
		const Point<int> _pt_top = FLOOD_STACK.top();
		FLOOD_STACK.pop();

		const Pattern& p = mTilePalette.pattern();
		mMap.getCellByGridCoords(_pt_top).index(layer, p.value(_pt_top.x % p.width, _pt_top.y % p.height));

		for (int i = 0; i < 4; i++)
		{
			Point<int> coord(_pt_top.x + dX[i], _pt_top.y + dY[i]);
			if (coord.x >= 0 && coord.x < mMap.width && coord.y >= 0 && coord.y < mMap.height && mMap.getCellByGridCoords(coord).index(layer) == seed_index)
			{
				FLOOD_STACK.push(coord);
			}
		}
	}
	*/
}


/**
* If value < 0, ignores the pattern values and writes -1 instead.
*/
void EditorState::pattern(int value)
{
	/*
	const Pattern* _p = &mTilePalette.pattern();
	if (value < 0) { _p = &mToolBar.brush(); }


	Point<int>& _pt = mMap.getGridCoords(mMouseCoords);

	for (int row = 0; row < _p->height(); row++)
	{
		for (int col = 0; col < _p->width(); col++)
		{
			int index = _p->value(col, row);
			int x = _pt.x - ((_p->width() - 1) - col);
			int y = _pt.y - ((_p->height() - 1) - row);

			if (x >= 0 && y >= 0)
			{
				if (value >= 0) mMap.getCellByGridCoords(x, y).index(layer, _p->value(col, row));
				else mMap.getCellByGridCoords(x, y).index(layer, -1);
			}
		}
	}
	*/
}


/**
 * Performs a pattern edit on collision layer.
 * 
 * \todo	Have this check the ToolBar for a pattern size
 * 			instead of using a pattern from the TilePalette.
 */
void EditorState::pattern_collision()
{
	/*
	const Pattern& _p = mToolBar.brush();
	Point<int>& _pt = mMap.getGridCoords(mMouseCoords);

	for (int row = 0; row < _p.height; row++)
	{
		for (int col = 0; col < _p.width; col++)
		{
			int x = _pt.x - ((_p.width - 1) - col);
			int y = _pt.y - ((_p.height - 1) - row);

			if (x >= 0 && y >= 0)
			{
				mMap.getCellByGridCoords(x, y).blocked(mPlacingCollision);
			}
		}
	}
	*/
}


void EditorState::toolbar_event(ToolBar::ToolBarAction _act)
{
	switch (_act)
	{
	case ToolBar::TOOLBAR_SAVE:
		saveMap();
		break;

	case ToolBar::TOOLBAR_MINIMAP_TOGGLE:
		mMiniMap.visible(mToolBar.show_minimap());
		break;

	case ToolBar::TOOLBAR_TILE_PALETTE_TOGGLE:
		mTilePalette.visible(mToolBar.show_tilepalette());
		break;

	case ToolBar::TOOLBAR_TILE_GROUPS_TOGGLE:
		mTileGroups.visible(mToolBar.show_tilegroups());
		break;

	case ToolBar::TOOLBAR_TOOL_PENCIL:
		Utility<Renderer>::get().setCursor(POINTER_NORMAL);
		break;

	case ToolBar::TOOLBAR_TOOL_FILL:
		Utility<Renderer>::get().setCursor(POINTER_FILL);
		break;

	case ToolBar::TOOLBAR_TOOL_ERASER:
		Utility<Renderer>::get().setCursor(POINTER_ERASE);
		break;

	case ToolBar::TOOLBAR_QUIT:
		mReturnState = new StartState();
		break;

	default:
		break;
	}
}


/**
 * Saves a map.
 * 
 * Displays the File Save Path entry screen if the map's save path is empty.
 */
void EditorState::saveMap()
{
	Filesystem& f = Utility<Filesystem>::get();

	if (!f.exists("maps")) { f.makeDirectory("maps"); }

	//mMap.save(mMapSavePath);
}


/**
 * Saves and undo level.
 */
void EditorState::saveUndo()
{
	//mFieldUndo = mMap.field();
}


/**
 * Draws debug information.
 */
void EditorState::debug()
{}
