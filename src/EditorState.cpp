#include "EditorState.h"
#include "StartState.h"

#include "Common.h"
#include "ConfigStrings.h"


const bool			SHOW_DEBUG_DEFAULT	= false;
const bool			HIDE_UI_DEFAULT		= false;

const float			SCROLL_SPEED		= 250.0f;


const float			MINI_MAP_X			= 10.0f;
const float			MINI_MAP_Y			= 65.0f;

SDL_Surface*		MINI_MAP_SURFACE	= NULL; // UGLY HACK!


std::map<EditState, string>	StateStringMap;		/**< EditState string table. */
std::map<int, EditState>	StateIntMap;		/**< EditState int table. */


EditorState::EditorState(const string& mapPath):	mMousePointer(Utility<Configuration>::get().option(CONFIG_UI_MOUSE_POINTER_IMAGE)),
													mFont(Utility<Configuration>::get().option(CONFIG_UI_MAIN_FONT_PATH), stringToInt(Utility<Configuration>::get().option(CONFIG_UI_MAIN_FONT_SIZE))),
													mTitleFont(Utility<Configuration>::get().option(CONFIG_UI_MAP_NAME_FONT_PATH), stringToInt(Utility<Configuration>::get().option(CONFIG_UI_MAP_NAME_FONT_SIZE))),
													mLinkCell(NULL),
													mMap(mapPath),
													mMiniMap(NULL),
													mMapSavePath(mapPath),
													mEditState(STATE_BASE_TILE_INDEX),
													mPreviousEditState(mEditState),
													mDrawDebug(SHOW_DEBUG_DEFAULT),
													mLeftButtonDown(false),
													mRightButtonDown(false),
													mPlacingCollision(false),
													mHideUi(HIDE_UI_DEFAULT),
													mDrawMiniMap(true),
													mMapChanged(false),
													mReturnState(NULL)
{}


EditorState::EditorState(const string& name, const string& tsetPath, int w, int h):	mMousePointer(Utility<Configuration>::get().option(CONFIG_UI_MOUSE_POINTER_IMAGE)),
																					mFont(Utility<Configuration>::get().option(CONFIG_UI_MAIN_FONT_PATH), stringToInt(Utility<Configuration>::get().option(CONFIG_UI_MAIN_FONT_SIZE))),
																					mTitleFont(Utility<Configuration>::get().option(CONFIG_UI_MAP_NAME_FONT_PATH), stringToInt(Utility<Configuration>::get().option(CONFIG_UI_MAP_NAME_FONT_SIZE))),
																					mLinkCell(NULL),
																					mMap(name, tsetPath, w, h),
																					mMiniMap(NULL),
																					mEditState(STATE_FILE_SAVE_PATH),
																					mPreviousEditState(STATE_BASE_TILE_INDEX),
																					mDrawDebug(SHOW_DEBUG_DEFAULT),
																					mLeftButtonDown(false),
																					mRightButtonDown(false),
																					mPlacingCollision(false),
																					mHideUi(HIDE_UI_DEFAULT),
																					mDrawMiniMap(true),
																					mMapChanged(false),
																					mReturnState(NULL)
{}


/**
 * Initializes internal values and sets up event handlers.
 */
void EditorState::initialize()
{
	mReturnState = this;

	// Tile Palette
	mTilePalette.tileset(&mMap.tileset());
	mTilePalette.font(mFont);


	// Layer Menu
	mLayerMenu.font(mFont);
	mLayerMenu.addItem("Base");
	mLayerMenu.addItem("Base Detail");
	mLayerMenu.addItem("Detail");
	mLayerMenu.addItem("Foreground");
	mLayerMenu.addItem("Collision");
	mLayerMenu.position(0, Utility<Renderer>::get().height() - mLayerMenu.rect().h());


	// Map Name Field
	mTxtMapName.font(mTitleFont);
	mTxtMapName.width(300);
	mTxtMapName.position(1, 1);
	mTxtMapName.text(mMap.name());


	// Map Save Path Field
	mTxtMapSavePath.font(mFont);
	mTxtMapSavePath.width(300);
	mTxtMapSavePath.position(Utility<Renderer>::get().screenCenterX() - 150, Utility<Renderer>::get().screenCenterY() - 10);
	mTxtMapSavePath.text(mMapSavePath);
	mTxtMapSavePath.border(TextField::ALWAYS);


	// Okay Button for Map Save Path state
	mBtnMapSavePathOkay.font(mFont);
	mBtnMapSavePathOkay.size(50, 25);
	mBtnMapSavePathOkay.position(Utility<Renderer>::get().screenCenterX() + 100, Utility<Renderer>::get().screenCenterY() + 20);
	mBtnMapSavePathOkay.text("Okay");
	mBtnMapSavePathOkay.click().Connect(this, &EditorState::button_MapSavePathOkay_Click);


	// Link Edit UI
	mBtnLinkOkay.font(mFont);
	mBtnLinkOkay.size(50, 25);
	mBtnLinkOkay.position(10, 160);
	mBtnLinkOkay.text("Okay");
	mBtnLinkOkay.click().Connect(this, &EditorState::button_MapLinkOkay_Click);
	mBtnLinkOkay.visible(false);

	mBtnLinkCancel.font(mFont);
	mBtnLinkCancel.size(50, 25);
	mBtnLinkCancel.position(75, 160);
	mBtnLinkCancel.text("Cancel");
	mBtnLinkCancel.click().Connect(this, &EditorState::button_MapLinkCancel_Click);
	mBtnLinkCancel.visible(false);


	mTxtLinkDestination.font(mFont);
	mTxtLinkDestination.width(300);
	mTxtLinkDestination.position(10, 100);
	mTxtLinkDestination.border(TextField::ALWAYS);
	mTxtLinkDestination.visible(false);

	mTxtLinkDestX.font(mFont);
	mTxtLinkDestX.width(100);
	mTxtLinkDestX.position(10, 130);
	mTxtLinkDestX.text("0");
	mTxtLinkDestX.border(TextField::ALWAYS);
	mTxtLinkDestX.visible(false);

	mTxtLinkDestY.font(mFont);
	mTxtLinkDestY.width(100);
	mTxtLinkDestY.position(150, 130);
	mTxtLinkDestY.text("0");
	mTxtLinkDestY.border(TextField::ALWAYS);
	mTxtLinkDestY.visible(false);

	// Set UI visibility based on the set editor state.
	if(mEditState == STATE_FILE_SAVE_PATH)
	{
		mTxtMapSavePath.visible(true);
		mTxtMapSavePath.hasFocus(true);

		mBtnMapSavePathOkay.visible(true);
	}
	else
	{
		mTxtMapSavePath.visible(false);
		mBtnMapSavePathOkay.visible(false);
	}


	// File tables
	fillEditStateTable();
	fillEditStateStringTable();


	// Hook up event handlers
	Utility<EventHandler>::get().keyUp().Connect(this, &EditorState::onKeyUp);
	Utility<EventHandler>::get().keyDown().Connect(this, &EditorState::onKeyDown);
	Utility<EventHandler>::get().mouseMotion().Connect(this, &EditorState::onMouseMove);
	Utility<EventHandler>::get().mouseButtonUp().Connect(this, &EditorState::onMouseUp);
	Utility<EventHandler>::get().mouseButtonDown().Connect(this, &EditorState::onMouseDown);
	Utility<EventHandler>::get().quit().Connect(this, &EditorState::onQuit);

	createMiniMap();
}


/**
 * Fills a table with EditStates and maps them to human-readable strings.
 */
void EditorState::fillEditStateTable()
{
	StateIntMap[0] = STATE_BASE_TILE_INDEX;
	StateIntMap[1] = STATE_BASE_DETAIL_TILE_INDEX;
	StateIntMap[2] = STATE_DETAIL_TILE_INDEX;
	StateIntMap[3] = STATE_FOREGROUND_TILE_INDEX;
	StateIntMap[4] = STATE_TILE_COLLISION;
}


/**
 * Fills a table with Ints and maps them to EditStates.
 */
void EditorState::fillEditStateStringTable()
{
	StateStringMap[STATE_BASE_TILE_INDEX]			= "Base Layer Editing";
	StateStringMap[STATE_BASE_DETAIL_TILE_INDEX]	= "Base Detail Layer Editing";
	StateStringMap[STATE_DETAIL_TILE_INDEX]			= "Detail Layer Editing";
	StateStringMap[STATE_FOREGROUND_TILE_INDEX]		= "Foreground Layer Editing";
	StateStringMap[STATE_TILE_COLLISION]			= "Collision Layer Editing";
	StateStringMap[STATE_FILE_SAVE_PATH]			= "Specifying File Save Path";
	StateStringMap[STATE_MAP_LINK_EDIT]				= "Map Link Editing";
	StateStringMap[STATE_MAP_NAME_EDIT]				= "Map Name Editing";
}


/**
 * Handler for the button's Click event.
 */
void EditorState::button_MapSavePathOkay_Click()
{
	setMapSavePath();
	mLinkCell = NULL;
}


/**
 * Handler link okay button click.
 * 
 * \warning	Does absolutely no error checking at all
 *			so make sure values are good.
 */
void EditorState::button_MapLinkOkay_Click()
{
	if(mLinkCell)
	{
		mLinkCell->link(mTxtLinkDestination.text());
		mLinkCell->link_destination(Point_2d(stringToInt(mTxtLinkDestX.text()), stringToInt(mTxtLinkDestY.text())));
	}

	mBtnLinkOkay.visible(false);
	mBtnLinkCancel.visible(false);
	mTxtLinkDestination.visible(false);
	mTxtLinkDestX.visible(false);
	mTxtLinkDestY.visible(false);

	mLinkCell = NULL;

	restorePreviousState();
}


/**
 * Handler for the button's Click event.
 */
void EditorState::button_MapLinkCancel_Click()
{
	restorePreviousState();

	mBtnLinkOkay.visible(false);
	mBtnLinkCancel.visible(false);
	mTxtLinkDestination.visible(false);
	mTxtLinkDestX.visible(false);
	mTxtLinkDestY.visible(false);
}


/**
 * 
 */
State* EditorState::update()
{
	Renderer& r = Utility<Renderer>::get();
	r.clearScreen(COLOR_BLACK);

	updateScroll();
	updateSelector();


	if(mHideUi)
		return mReturnState;

	if(mDrawDebug)
		debug();

	if(mDrawMiniMap)
		drawMiniMap();

	if(mEditState == STATE_FILE_SAVE_PATH)
	{
		r.drawBoxFilled(0, 0, r.width(), r.height(), 0, 0, 0, 65);
		r.drawText(mFont, "Enter file name for Map:", r.screenCenterX() - 150, r.screenCenterY() - 30, 255, 255, 255);
		mTxtMapSavePath.update();
		mBtnMapSavePathOkay.update();
	}

	if(mEditState == STATE_MAP_LINK_EDIT)
	{
		r.drawBoxFilled(0, 0, r.width(), r.height(), 0, 0, 0, 65);
		r.drawBox(mCellInspectRect, 255, 255, 0);
	}

	updateUI();

	if(mEditState != STATE_FILE_SAVE_PATH)
		r.drawTextShadow(mFont, "Map File: " + mMapSavePath, r.screenCenterX() - (mFont.width("Map File: " + mMapSavePath) / 2), r.height() - (mFont.height() + 2), 1, 255, 255, 255, 0, 0, 0);

	r.drawImage(mMousePointer, mMouseCoords.x(), mMouseCoords.y());

	return mReturnState;
}


void EditorState::updateUI()
{
	instructions();

	stringstream str;
	str << "World Tile: " << static_cast<int>((mMouseCoords.x() + mMap.cameraPosition().x()) / mMap.tileset().width()) << ", " << static_cast<int>((mMouseCoords.y() + mMap.cameraPosition().y()) / mMap.tileset().height());
	Utility<Renderer>::get().drawTextShadow(mFont, str.str(), 5, mLayerMenu.rect().y() - 30, 1, 255, 255, 255, 0, 0, 0);

	str.str("");
	str << "World Fine: " << mMouseCoords.x() + mMap.cameraPosition().x() << ", " << mMouseCoords.y() + mMap.cameraPosition().y();
	Utility<Renderer>::get().drawTextShadow(mFont, str.str(), 5, mLayerMenu.rect().y() - 20, 1, 255, 255, 255, 0, 0, 0);


	mLayerMenu.update();
	mTilePalette.update();
	mTxtMapName.update();

	mBtnLinkOkay.update();
	mBtnLinkCancel.update();

	mTxtLinkDestination.update();
	mTxtLinkDestX.update();
	mTxtLinkDestY.update();
}


void EditorState::drawMiniMap()
{
	Renderer& r = Utility<Renderer>::get();

	Point_2d pt(0, 0);

	Point_2d upperLeft = mMap.getGridCoords(pt);
	pt.x() += r.width();
	pt.y() += r.height();
	Point_2d lowerRight = mMap.getGridCoords(pt);

	r.drawImage(*mMiniMap, MINI_MAP_X, MINI_MAP_Y);

	Rectangle_2d rect(MINI_MAP_X + upperLeft.x(), MINI_MAP_Y + upperLeft.y(), lowerRight.x() - upperLeft.x(), lowerRight.y() - upperLeft.y());
	r.drawBox(rect, 255, 255, 255);
}


void EditorState::createMiniMap()
{
	Uint32 rmask, gmask, bmask, amask;
	// Set up channel masks.
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		rmask = 0xff000000; gmask = 0x00ff0000;	bmask = 0x0000ff00;	amask = 0x000000ff;
	}
	else
	{
		rmask = 0x000000ff;	gmask = 0x0000ff00;	bmask = 0x00ff0000;	amask = 0xff000000;
	}

	if(MINI_MAP_SURFACE)
		SDL_FreeSurface(MINI_MAP_SURFACE);

	MINI_MAP_SURFACE = SDL_CreateRGBSurface(0, mMap.width(), mMap.height(), 32, rmask, gmask, bmask, amask);

	if(!MINI_MAP_SURFACE)
		return;


	Color_4ub _c;
	for(int y = 0; y < mMap.height(); y++)
	{
		for(int x = 0; x < mMap.width(); x++)
		{
			Cell& cell = mMap.field().cell(x, y);

			_c = mMap.tileset().averageColor(cell.index(Cell::LAYER_BASE));
			DrawPixel(MINI_MAP_SURFACE, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());

			if(cell.index(Cell::LAYER_BASE_DETAIL) != -1)
			{
				_c = mMap.tileset().averageColor(cell.index(Cell::LAYER_BASE_DETAIL));
				DrawPixel(MINI_MAP_SURFACE, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());
			}

			if(cell.index(Cell::LAYER_DETAIL) != -1)
			{
				_c = mMap.tileset().averageColor(cell.index(Cell::LAYER_DETAIL));
				DrawPixel(MINI_MAP_SURFACE, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());
			}

			if(cell.index(Cell::LAYER_FOREGROUND) != -1)
			{
				_c = mMap.tileset().averageColor(cell.index(Cell::LAYER_FOREGROUND));
				DrawPixel(MINI_MAP_SURFACE, x, y, _c.red(), _c.green(), _c.blue(), _c.alpha());
			}
		}
	}


	if(mMiniMap)
		delete mMiniMap;

	mMiniMap = new Image(MINI_MAP_SURFACE->pixels, MINI_MAP_SURFACE->format->BytesPerPixel, MINI_MAP_SURFACE->w, MINI_MAP_SURFACE->h);
}


/**
 * Updates the map scrolling.
 * 
 * Also updates the current tick time.
 */
void EditorState::updateScroll()
{
	mMap.update();
	if(!mHideUi || mEditState != STATE_FILE_SAVE_PATH)
	{
		mSelectorRect = mMap.injectMousePosition(mMouseCoords);
	}

	float delta = mTimer.delta();
	mMap.moveCamera(static_cast<float>(mScrollVector.x()) * delta, static_cast<float>(mScrollVector.y()) * delta);
}


/**
 * Draws the tile selector based on the pattern selected in the TilePalette.
 */
void EditorState::updateSelector()
{
	// Don't draw selector if the UI is hidden.
	if(mHideUi)
		return;

	Renderer& r = Utility<Renderer>::get();

	// Draw Tile Selector
	int offsetX = 0, offsetY = 0;
	const Pattern& p = mTilePalette.pattern();
	for(int row = p.height(); row > 0; row--)
	{
		for(int col = p.width(); col > 0; col--)
		{
			r.drawBox(mSelectorRect.x() - offsetX, mSelectorRect.y() - offsetY, mSelectorRect.w(), mSelectorRect.h(), 255, 255, 255);
			offsetX += 32;
		}
		offsetX = 0;
		offsetY += 32;
	}
}


/**
 * Handles KeyDown events.
 */
void EditorState::onKeyDown(KeyCode key, KeyModifier mod, bool repeat)
{
	if(repeat)
		return;

	if(mEditState == STATE_MAP_NAME_EDIT)
	{
		// User pressed enter while in the map name text field,
		// turn focus off.
		if(key == KEY_ENTER || key == KEY_KP_ENTER)
		{
			mTxtMapName.hasFocus(false);
			restorePreviousState();
		}
		return;
	}
	else if(mEditState == STATE_FILE_SAVE_PATH)
	{
		if(key == KEY_ENTER || key == KEY_KP_ENTER)
			setMapSavePath();
		else if(key == KEY_ESCAPE)
			mReturnState = new StartState();
	
		return;
	}
	else if(mEditState == STATE_MAP_LINK_EDIT)
	{
		return;
	}

	stringstream str;
	//int x, y;

	switch(key)
	{
		case KEY_ESCAPE:
			mReturnState = new StartState();
			break;

		case KEY_LEFT:
			mScrollVector.x() = -SCROLL_SPEED;
			break;

		case KEY_RIGHT:
			mScrollVector.x() = SCROLL_SPEED;
			break;

		case KEY_UP:
			mScrollVector.y() = -SCROLL_SPEED;
			break;

		case KEY_DOWN:
			mScrollVector.y() = SCROLL_SPEED;
			break;

		case KEY_1:
			setState(STATE_BASE_TILE_INDEX);
			mLayerMenu.currentSelection(0);
			mMap.drawCollision(false);
			break;

		case KEY_2:
			setState(STATE_BASE_DETAIL_TILE_INDEX);
			mLayerMenu.currentSelection(1);
			mMap.drawCollision(false);
			break;

		case KEY_3:
			setState(STATE_DETAIL_TILE_INDEX);
			mLayerMenu.currentSelection(2);
			mMap.drawCollision(false);
			break;

		case KEY_4:
			setState(STATE_FOREGROUND_TILE_INDEX);
			mLayerMenu.currentSelection(3);
			mMap.drawCollision(false);
			break;

		case KEY_5:
			setState(STATE_TILE_COLLISION);
			mLayerMenu.currentSelection(4);
			mMap.drawCollision(true);
			break;

		case KEY_F1:
			mDrawDebug ? mDrawDebug = false : mDrawDebug = true;
			if(mDrawDebug)
				mMap.showLinks(true);
			else
				mMap.showLinks(false);
			break;

		// Stealing this for dumping the minimap for now
		case KEY_F2:
			/*
			if(!Utility<Filesystem>::get().exists("screenshots"))
				Utility<Filesystem>::get().makeDirectory("screenshots");

			mMap.dump("screenshots/map_dump.bmp");
			*/

			SDL_SaveBMP(MINI_MAP_SURFACE, "minimap.bmp");

			break;

		case KEY_F3:
			mBtnLinkOkay.visible(true);
			mBtnLinkCancel.visible(true);
			mTxtLinkDestination.visible(true);
			mTxtLinkDestX.visible(true);
			mTxtLinkDestY.visible(true);
			mLinkCell = &mMap.getCell(mMouseCoords);
			mCellInspectRect = mMap.injectMousePosition(mMouseCoords);
			mTxtLinkDestination.text(mLinkCell->link());
			str << mLinkCell->link_destination().x();
			mTxtLinkDestX.text(str.str());
			str.str("");
			str << mLinkCell->link_destination().y();
			mTxtLinkDestY.text(str.str());
			setState(STATE_MAP_LINK_EDIT);
			break;

		case KEY_F4:
			saveMap();
			break;

		case KEY_F5:
			mMap.toggleBgDetail();
			break;

		case KEY_F6:
			mMap.toggleDetail();
			break;

		case KEY_F7:
			mMap.toggleForeground();
			break;

		case KEY_F10:
			mHideUi ? mHideUi = false : mHideUi = true;
			break;

		case KEY_m:
			mDrawMiniMap ? mDrawMiniMap = false : mDrawMiniMap = true;
			break;

		case KEY_z:
			if(KeyTranslator::control(mod))
			{
				if(!mFieldUndo.empty())
				{
					mMap.field(mFieldUndo);
					createMiniMap();
				}
			}

			break;

		default:
			break;

	}
}


/**
 * Handles KeyUp events.
 */
void EditorState::onKeyUp(KeyCode key, KeyModifier mod)
{
	if(mTxtMapName.hasFocus())
	{
		return;
	}

	if(mEditState == STATE_FILE_SAVE_PATH)
	{
		return;
	}

	if(mEditState == STATE_MAP_LINK_EDIT)
	{
		return;
	}

	if(key == KEY_LEFT)
		mScrollVector.x() = 0.0f;
	else if(key == KEY_RIGHT)
		mScrollVector.x() = 0.0f;
	else if(key == KEY_UP)
		mScrollVector.y() = 0.0f;
	else if(key == KEY_DOWN)
		mScrollVector.y() = 0.0f;
}


/**
 *
 */
void EditorState::onQuit()
{
	mReturnState = NULL;
}


/**
 *
 */
void EditorState::onMouseMove(int x, int y, int relX, int relY)
{
	if(mRightButtonDown && mEditState != STATE_MAP_LINK_EDIT)
	{
		mMap.moveCamera(relX, relY);
	}
	else
	{
		mMouseCoords(x, y);

		mTilePalette.onMouseMove(x, y, relX, relY);

		if(mLeftButtonDown && !isPointInRect(mMouseCoords, mTilePalette.rect()) && !mTxtMapName.hasFocus())
		{
			if(mEditState != STATE_MAP_LINK_EDIT && isPointInRect(x, y, MINI_MAP_X, MINI_MAP_Y, mMiniMap->rect().w(), mMiniMap->rect().h()))
			{
				if(!mDrawMiniMap)
					return;

				mMap.setCamera((mMap.tileset().width() * (x - MINI_MAP_X)) - (Utility<Renderer>::get().width() / 2), (mMap.tileset().height() * (y - MINI_MAP_Y)) - (Utility<Renderer>::get().height() / 2));
				return;
			}

			if(mEditState == STATE_TILE_COLLISION)
				mMap.getCell(mMouseCoords).blocked(mPlacingCollision);
			else
				changeTileTexture();
		}
	}
}


/**
 * Handles MouseDown events.
 */
void EditorState::onMouseDown(MouseButton button, int x, int y)
{
	Utility<EventHandler>::get().grabMouse();

	// Left Mouse Button
	if(button == BUTTON_LEFT)
	{
		mLeftButtonDown = true;
		handleLeftButtonDown(x, y);
	}
	else if(button == BUTTON_RIGHT)
	{
		handleRightButtonDown();
	}
	else if(button == BUTTON_MIDDLE)
	{
		handleMiddleButtonDown();
	}
}


/**
 * Handles MouseUp events
 */
void EditorState::onMouseUp(MouseButton button, int x, int y)
{
	if(button == BUTTON_LEFT)
	{
		mLeftButtonDown = false;
		mTilePalette.onMouseUp(button, x, y);

		if(mEditState == STATE_MAP_LINK_EDIT)
		{
		}
		else if(mEditState == STATE_BASE_TILE_INDEX || mEditState == STATE_BASE_DETAIL_TILE_INDEX || mEditState == STATE_DETAIL_TILE_INDEX || mEditState == STATE_FOREGROUND_TILE_INDEX)
		{
			if(mMapChanged)
			{
				createMiniMap();
				mMapChanged = false;
			}
		}
	}
	else if(button == BUTTON_RIGHT)
	{
		mRightButtonDown = false;
		Utility<EventHandler>::get().mouseRelativeMode(false);
	}

	Utility<EventHandler>::get().releaseMouse();
}


/**
 * Handles left mouse button down.
 */
void EditorState::handleLeftButtonDown(int x, int y)
{
	Point_2d pt(x, y);

	if(mEditState != STATE_MAP_LINK_EDIT && mDrawMiniMap && isPointInRect(x, y, MINI_MAP_X, MINI_MAP_Y, mMiniMap->rect().w(), mMiniMap->rect().h()))
	{
		mMap.setCamera((mMap.tileset().width() * (x - MINI_MAP_X)) - (Utility<Renderer>::get().width() / 2), (mMap.tileset().height() * (y - MINI_MAP_Y)) - (Utility<Renderer>::get().height() / 2));
		return;
	}

	if(mEditState == STATE_FILE_SAVE_PATH)
	{
		return;
	}

	if(mEditState == STATE_MAP_LINK_EDIT)
	{
		return;
	}

	if(isPointInRect(pt, mTxtMapName.rect()))
	{
		mTxtMapName.hasFocus(true);
		setState(STATE_MAP_NAME_EDIT);
		return;
	}
	else
	{
		mTxtMapName.hasFocus(false);
		if(mEditState == STATE_MAP_NAME_EDIT)
			restorePreviousState();
	}


	// Check the layer menu
	if(isPointInRect(pt, mLayerMenu.rect()))
	{
		setState(StateIntMap[mLayerMenu.currentSelection()]);
		mEditState == STATE_TILE_COLLISION ? mMap.drawCollision(true) : mMap.drawCollision(false);
		return; // Ignore other click processing if the menu was clicked.
	}


	// Check the tile palette.
	if(mTilePalette.hidden() && isPointInRect(pt, mTilePalette.rect()))
	{
		showTilePalette();
		return; // Ignore other click processing if the palette was clicked.
	}

	if(!mTilePalette.hidden() && isPointInRect(pt, mTilePalette.closeRect()))
	{
		hideTilePalette();
		return; // Ignore other click processing if the palette was clicked.
	}


	Cell& cell = mMap.getCell(mMouseCoords);

	if(mEditState == STATE_TILE_COLLISION)
	{
		saveUndo();
		if(cell.blocked())
		{
			cell.blocked(false);
			mPlacingCollision = false;
		}
		else
		{
			cell.blocked(true);
			mPlacingCollision = true;
		}
	}
	else if(isPointInRect(pt, mTilePalette.rect()))
	{
		mTilePalette.onMouseDown(BUTTON_LEFT, x, y);
	}
	else
	{
		saveUndo();
		changeTileTexture();
	}
}


void EditorState::handleRightButtonDown()
{
	mRightButtonDown = true;
	Utility<EventHandler>::get().mouseRelativeMode(true);
}


/**
 * Handles middle button presses.
 */
void EditorState::handleMiddleButtonDown()
{
	saveUndo();

	switch(mEditState)
	{
		case STATE_BASE_DETAIL_TILE_INDEX:
			mMap.getCell(mMouseCoords).index(Cell::LAYER_BASE_DETAIL, -1);
			break;

		case STATE_DETAIL_TILE_INDEX:
			mMap.getCell(mMouseCoords).index(Cell::LAYER_DETAIL, -1);
			break;

		case STATE_FOREGROUND_TILE_INDEX:
			mMap.getCell(mMouseCoords).index(Cell::LAYER_FOREGROUND, -1);
			break;

		default:
			break;
	}

	// Update the minimap after tile erase.
	if(mEditState == STATE_BASE_DETAIL_TILE_INDEX || mEditState == STATE_DETAIL_TILE_INDEX || mEditState == STATE_FOREGROUND_TILE_INDEX)
		createMiniMap();
}


/**
 * Changes the tile texture index of the highlighted Cell.
 * 
 * \note	Will not modify tile texture index unless in the correct edit mode.
 */
void EditorState::changeTileTexture()
{
	switch(mEditState)
	{
		case STATE_BASE_TILE_INDEX:
			if(mTilePalette.patternFill())
				patternFill(Cell::LAYER_BASE);
			else
				patternStamp(Cell::LAYER_BASE);
			break;

		case STATE_BASE_DETAIL_TILE_INDEX:
			if(mTilePalette.patternFill())
				patternFill(Cell::LAYER_BASE_DETAIL);
			else
				patternStamp(Cell::LAYER_BASE_DETAIL);
			break;

		case STATE_DETAIL_TILE_INDEX:
			if(mTilePalette.patternFill())
				patternFill(Cell::LAYER_DETAIL);
			else
				patternStamp(Cell::LAYER_DETAIL);
			break;

		case STATE_FOREGROUND_TILE_INDEX:
			if(mTilePalette.patternFill())
				patternFill(Cell::LAYER_FOREGROUND);
			else
				patternStamp(Cell::LAYER_FOREGROUND);
			break;

		default:
			break;
	}
}


/**
 * Fills a given cell layer with a pattern.
 *
 * \todo	Implement actual pattern filling.
 */
void EditorState::patternFill(Cell::TileLayer layer)
{
	const Pattern& p = mTilePalette.pattern();

	for(int row = 0; row < mMap.height(); row++)
		for(int col = 0; col < mMap.width(); col++)
			mMap.getCellByGridCoords(col, row).index(layer, p.value(col % p.width(), row % p.height()));

	mMapChanged = true;
}


/**
 * Stamps a pattern onto a given layer.
 */
void EditorState::patternStamp(Cell::TileLayer layer)
{
	Point_2d& pt = mMap.getGridCoords(mMouseCoords);

	const Pattern& p = mTilePalette.pattern();

	for(int row = 0; row < p.height(); row++)
	{
		for(int col = 0; col < p.width(); col++)
		{
			int index = p.value(col, row);
			int x = pt.x() - ((p.width() - 1) - col);
			int y = pt.y() - ((p.height() - 1) - row);

			if(x >= 0 && y >= 0)
				mMap.getCellByGridCoords(x, y).index(layer, p.value(col, row));
		}
	}

	mMapChanged = true;
}



/**
 * Changes the edit mode and unhides the tile palette.
 */
void EditorState::showTilePalette()
{
	mTilePalette.hidden(false);
}


/**
 * Changes the edit mode and hides the tile palette.
 */
void EditorState::hideTilePalette()
{
	mTilePalette.hidden(true);
}


/**
 * Saves a map.
 * 
 * Displays the File Save Path entry screen if the map's save path is empty.
 */
void EditorState::saveMap()
{
	if(mMapSavePath.empty())
	{
		mPreviousEditState = mEditState;
		setState(STATE_FILE_SAVE_PATH);
		mTxtMapSavePath.visible(true);
		mTxtMapSavePath.hasFocus(true);
	}
	else
	{
		mMap.name(mTxtMapName.text());
		mMap.save(mMapSavePath);
	}
}


/**
 * Saves and undo level.
 */
void EditorState::saveUndo()
{
	mFieldUndo = mMap.field();
}


/**
 * Sets the state of the editor once the user presses enter/
 * clicks the Okay button.
 */
void EditorState::setMapSavePath()
{
	string str = TrimString(mTxtMapSavePath.text());

	if(str.empty())
		return;

	restorePreviousState();
	mTxtMapSavePath.visible(false);
	mBtnMapSavePathOkay.visible(false);
	mMapSavePath = Utility<Configuration>::get().option(CONFIG_EDITOR_MAPS_PATH) + str;
}


/**
 * Sets the current state and saves the previous state.
 */
void EditorState::setState(EditState state)
{
	if(mEditState == state)
		return;

	mPreviousEditState = mEditState;
	mEditState = state;
}


/**
 * Restores the previous state.
 */
void EditorState::restorePreviousState()
{
	EditState state = mEditState;
	mEditState = mPreviousEditState;
	mPreviousEditState = state;
}


/**
 * Draws debug information.
 */
void EditorState::debug()
{
	Renderer& r = Utility<Renderer>::get();

	// Cell coords pointed at by mouse.
	std::stringstream ss;
	Point_2d pt = mMap.getGridCoords(mMouseCoords);
	ss << "Cell Coords: " << pt.x() << ", " << pt.y();
	r.drawTextShadow(mFont, ss.str(), 4, 100, 1, 255, 255, 255, 0, 0, 0);


	// Edit State
	ss.str("");
	ss.str("Edit State: " + StateStringMap[mEditState]);
	r.drawTextShadow(mFont, ss.str(), 4, 115, 1, 255, 255, 255, 0, 0, 0);

	// Tile Index
	Cell& cell = mMap.getCell(mMouseCoords);

	ss.str("");
	ss << "Base: " << cell.index(Cell::LAYER_BASE);
	r.drawTextShadow(mFont, ss.str(), 4, 145, 1, 255, 255, 255, 0, 0, 0);

	ss.str("");
	ss << "Base Detail: " << cell.index(Cell::LAYER_BASE_DETAIL);
	r.drawTextShadow(mFont, ss.str(), 4, 160, 1, 255, 255, 255, 0, 0, 0);

	ss.str("");
	ss << "Detail: " << cell.index(Cell::LAYER_DETAIL);
	r.drawTextShadow(mFont, ss.str(), 4, 175, 1, 255, 255, 255, 0, 0, 0);

	ss.str("");
	ss << "Foreground: " << cell.index(Cell::LAYER_FOREGROUND);
	r.drawTextShadow(mFont, ss.str(), 4, 190, 1, 255, 255, 255, 0, 0, 0);

	ss.str("");
	cell.blocked() ? ss << "Blocked: true" : ss << "Blocked: false";
	r.drawTextShadow(mFont, ss.str(), 4, 205, 1, 255, 255, 255, 0, 0, 0);

	ss.str("");
	ss << "Link: " << cell.link();
	r.drawTextShadow(mFont, ss.str(), 4, 235, 1, 255, 255, 255, 0, 0, 0);

	ss.str("");
	ss << "Destination: " << cell.link_destination().x() << ", " << cell.link_destination().y();
	r.drawTextShadow(mFont, ss.str(), 4, 250, 1, 255, 255, 255, 0, 0, 0);

	mMap.tileset().drawTileColorPalette(r.width() - 32, 50, 4, 6);
}


/**
 * Draws a key mapping string.
 */
void EditorState::instructions()
{
	string str1 = "F1: Show/Hide Debug | F3: Map Link | F4: Save | F5: BG Detail | F6: Detail | F7: Foreground | F10: Hide/Show UI";
	Utility<Renderer>::get().drawTextShadow(mFont, str1, Utility<Renderer>::get().width() - mFont.width(str1) - 4, 4, 1, 255, 255, 255, 0, 0, 0);
}

