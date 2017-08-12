#include "EditorState.h"
#include "StartState.h"

#include "Common.h"

#include <stack>
#include <sstream>

using namespace NAS2D;


const bool			SHOW_DEBUG_DEFAULT	= false;
const bool			HIDE_UI_DEFAULT		= false;

const float			SCROLL_SPEED		= 250.0f;

SDL_Surface*		MINI_MAP_SURFACE	= nullptr; // HACK!


std::stack<Point_2d> FLOOD_STACK;		// Stack used for contiguous flood fill.


EditorState::EditorState(const string& mapPath):	mFont("fonts/opensans.ttf", 12),
													mBoldFont("fonts/opensans-bold.ttf", 12),
													mMapSavePath(mapPath),
													mReturnState(this)
{
}


EditorState::~EditorState()
{
	Utility<EventHandler>::get().keyUp().disconnect(this, &EditorState::onKeyUp);
	Utility<EventHandler>::get().keyDown().disconnect(this, &EditorState::onKeyDown);
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &EditorState::onMouseMove);
	Utility<EventHandler>::get().mouseButtonUp().disconnect(this, &EditorState::onMouseUp);
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &EditorState::onMouseDown);
	Utility<EventHandler>::get().quit().disconnect(this, &EditorState::onQuit);
	Utility<EventHandler>::get().windowResized().disconnect(this, &EditorState::onWindowResized);

	if (MINI_MAP_SURFACE)
	{
		SDL_FreeSurface(MINI_MAP_SURFACE);
		MINI_MAP_SURFACE = nullptr;
	}
}

/**
 * Initializes internal values and sets up event handlers.
 */
void EditorState::initialize()
{
	initUi();

	// Hook up event handlers
	Utility<EventHandler>::get().keyUp().connect(this, &EditorState::onKeyUp);
	Utility<EventHandler>::get().keyDown().connect(this, &EditorState::onKeyDown);
	Utility<EventHandler>::get().mouseMotion().connect(this, &EditorState::onMouseMove);
	Utility<EventHandler>::get().mouseButtonUp().connect(this, &EditorState::onMouseUp);
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &EditorState::onMouseDown);
	Utility<EventHandler>::get().quit().connect(this, &EditorState::onQuit);
	Utility<EventHandler>::get().windowResized().connect(this, &EditorState::onWindowResized);


	Renderer& r = Utility<Renderer>::get();
	mMap = new MapFile(mMapSavePath, MapFile::TileGroups);
	mMap->updateCameraAnchorArea(r.width(), r.height() - mToolBar.height());

}


void EditorState::initUi()
{
	// Tile Palette
	mTilePalette.font(mFont);
	mTilePalette.boldFont(mBoldFont);

	// ToolBar
	mToolBar.toolbar_event().connect(this, &EditorState::toolbar_event);

	// Mini Map
	mMiniMap.font(mFont);
	mMiniMap.boldFont(mBoldFont);
	mMiniMap.hidden(!mToolBar.show_minimap());
}


/**
 * 
 */
State* EditorState::update()
{
	Renderer& r = Utility<Renderer>::get();
	r.clearScreen(COLOR_MAGENTA);

	mMap->draw(0, 32, r.width(), r.height() - mToolBar.height());

	updateSelector();

	if(mDrawDebug)
		debug();

	updateUI();

	r.drawTextShadow(mBoldFont, "Map File: " + mMapSavePath, 5.0f, r.height() - mBoldFont.height(), 1, 255, 255, 255, 0, 0, 0);

	return mReturnState;
}


void EditorState::updateUI()
{
	Renderer& r = Utility<Renderer>::get();

	mToolBar.update();
	mMiniMap.update();

	r.drawTextShadow(mBoldFont, NAS2D::string_format("World Tile: %i, %i", static_cast<int>((mMouseCoords.x() + mMap->cameraPosition().x()) / TILE_SIZE), static_cast<int>((mMouseCoords.y() + mMap->cameraPosition().y() - TILE_SIZE) / TILE_SIZE)), 5, r.height() - 30, 1, 255, 255, 255, 0, 0, 0);

	mTilePalette.update();
}


/**
 * Draws the tile selector based on the pattern selected in the TilePalette.
 */
void EditorState::updateSelector()
{
	return;

	// Don't draw selector if the UI is hidden.
	if(mMouseCoords.y() < 32)
		return;

	if (mTilePalette.responding_to_events() || mMiniMap.responding_to_events())
		return;

	Renderer& r = Utility<Renderer>::get();

	// Draw Tile Selector
	int offsetX = 0, offsetY = 0;
	
	const Pattern* p = nullptr;
	if(mToolBar.erase())
		p = &mToolBar.brush();

	for(int row = p->height(); row > 0; row--)
	{
		for(int col = p->width(); col > 0; col--)
		{
			//r.drawBox(mSelectorRect.x() - offsetX + mMap.viewport().x(), mSelectorRect.y() - offsetY + mMap.viewport().y(), mSelectorRect.width(), mSelectorRect.height(), 255, 255, 255);
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
	if(repeat)
		return;

	switch(key)
	{
		case EventHandler::KEY_ESCAPE:
			mReturnState = new StartState();
			break;

		case EventHandler::KEY_F1:
			mDrawDebug = !mDrawDebug;
			break;

		// Stealing this for dumping the minimap for now
		case EventHandler::KEY_F2:
			SDL_SaveBMP(MINI_MAP_SURFACE, "minimap.bmp");
			break;

		case EventHandler::KEY_z:
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

	mMouseCoords(x, y);

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
	if(button == EventHandler::BUTTON_LEFT)
	{
		mLeftButtonDown = true;
		handleLeftButtonDown(x, y);
	}
	else if(button == EventHandler::BUTTON_RIGHT)
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
	if(button == EventHandler::BUTTON_LEFT)
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
	else if(button == EventHandler::BUTTON_RIGHT)
	{
		mRightButtonDown = false;
		Utility<EventHandler>::get().mouseRelativeMode(false);
		Utility<EventHandler>::get().warpMouse(mSavedMouseCoords.x(), mSavedMouseCoords.y()); // a bit hacky but does the job
	}

	Utility<EventHandler>::get().releaseMouse();
}


/**
 * Handles left mouse button down.
 */
void EditorState::handleLeftButtonDown(int x, int y)
{
	Point_2d pt(x, y);

	// Hate the look of this but it effectively condenses the ignore checks.
	if (y < 32 ||
		(mToolBar.flood() && isPointInRect(pt, mToolBar.flood_tool_extended_area())) ||
		isPointInRect(pt, mTilePalette.rect()) ||
		isPointInRect(pt, mMiniMap.rect()) ||
		isPointInRect(pt, mTilePalette.rect()))
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

	for (int row = 0; row < mMap.height(); row++)
	{
		for (int col = 0; col < mMap.width(); col++)
		{
			mMap.getCellByGridCoords(col, row).index(layer, p.value(col % p.width(), row % p.height()));
		}
	}
	*/
}


/**
 * Fills a contiguous area in a given layer with a pattern.
 */
void EditorState::patternFill_Contig(const Point_2d& _pt, int seed_index)
{
	/*
	const Pattern& _pCheck = mTilePalette.pattern();
	if (seed_index == _pCheck.value(_pt.x() % _pCheck.width(), _pt.y() % _pCheck.height()))
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
		const Point_2d _pt_top = FLOOD_STACK.top();
		FLOOD_STACK.pop();

		const Pattern& p = mTilePalette.pattern();
		mMap.getCellByGridCoords(_pt_top).index(layer, p.value(_pt_top.x() % p.width(), _pt_top.y() % p.height()));

		for (int i = 0; i < 4; i++)
		{
			Point_2d coord(_pt_top.x() + dX[i], _pt_top.y() + dY[i]);
			if (coord.x() >= 0 && coord.x() < mMap.width() && coord.y() >= 0 && coord.y() < mMap.height() && mMap.getCellByGridCoords(coord).index(layer) == seed_index)
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


	Point_2d& _pt = mMap.getGridCoords(mMouseCoords);

	for (int row = 0; row < _p->height(); row++)
	{
		for (int col = 0; col < _p->width(); col++)
		{
			int index = _p->value(col, row);
			int x = _pt.x() - ((_p->width() - 1) - col);
			int y = _pt.y() - ((_p->height() - 1) - row);

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
	Point_2d& _pt = mMap.getGridCoords(mMouseCoords);

	for (int row = 0; row < _p.height(); row++)
	{
		for (int col = 0; col < _p.width(); col++)
		{
			int x = _pt.x() - ((_p.width() - 1) - col);
			int y = _pt.y() - ((_p.height() - 1) - row);

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
		mMiniMap.hidden(!mToolBar.show_minimap());
		break;

	case ToolBar::TOOLBAR_TILE_PALETTE_TOGGLE:
		mTilePalette.hidden(!mToolBar.show_tilepalette());
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
