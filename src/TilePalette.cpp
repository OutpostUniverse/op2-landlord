#include "TilePalette.h"

#include "Common.h"

const Point_2d		PALETTE_DIMENSIONS		= Point_2d(196, 300);

const Point_2d		FIRST_TILE_COORDINATE	= Point_2d(2, 18);
const Point_2d		TILE_SLOT_PADDING		= Point_2d(0, 0);

const Point_2d		TILE_GRID_DIMENSIONS	= Point_2d(6, 8);

const int			NUM_TILES_PER_PAGE		= TILE_GRID_DIMENSIONS.x() * TILE_GRID_DIMENSIONS.y();

using namespace NAS2D;

/**
 * C'Tor
 */
TilePalette::TilePalette():	mRect(Utility<Renderer>::get().width() - PALETTE_DIMENSIONS.x()- 2, Utility<Renderer>::get().height() - PALETTE_DIMENSIONS.y() - 2, PALETTE_DIMENSIONS.x(), PALETTE_DIMENSIONS.y())
{
	init();
}


TilePalette::~TilePalette()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().disconnect(this, &TilePalette::onMouseDown);
	e.mouseButtonUp().disconnect(this, &TilePalette::onMouseUp);
	e.mouseMotion().disconnect(this, &TilePalette::onMouseMove);
}

void TilePalette::init()
{
	int yPosition = rect().y() + rect().height() - 23;

	// Hook events
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().connect(this, &TilePalette::onMouseDown);
	e.mouseButtonUp().connect(this, &TilePalette::onMouseUp);
	e.mouseMotion().connect(this, &TilePalette::onMouseMove);
}


/**
 * Sets the internal font to use.
 */
void TilePalette::font(Font& font)
{
	mFont = &font;
}


void TilePalette::boldFont(Font& font)
{
	mBoldFont = &font;
}


/**
 * Hides or unhides the TilePalette.
 * 
 * \note	If the Tileset pointer is NULL, TilePalette will assume a
 *			non-functional state and will refuse to unhide itself.
 */
void TilePalette::hidden(bool hidden)
{
	mHidden = hidden;
}


/**
 * Draws the tile palette
 */
void TilePalette::update()
{
	if (hidden())
		return;

	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), 180, 180, 180);
	r.drawBoxFilled(rect().x(), rect().y(), rect().width(), 16, 75, 95, 130);
	r.drawBox(rect(), 0, 0, 0);

	if (mFont)
	{
		r.drawText(*mBoldFont, "Tile Palette", mRect.x() + (mRect.width() / 2) - (mBoldFont->width("Tile Palette") / 2), mRect.y(), 255, 255, 255);
	}

}


void TilePalette::onMouseMove(int x, int y, int relX, int relY)
{
	mMouseCoords(x, y);

	if(hidden())
		return;

	if (mDragging)
	{
		mRect.x(mRect.x() + relX);
		mRect.y(mRect.y() + relY);

		return;
	}
}


void TilePalette::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (hidden() || (button != EventHandler::BUTTON_LEFT)) { return; }

	if (isPointInRect(x, y, rect().x(), rect().y(), rect().width(), 17))
	{
		mDragging = true;
		return;
	}

	if (!(isPointInRect(mMouseCoords, rect())))
	{
		return;
	}

	mLeftButtonDown = true;
}


void TilePalette::onMouseUp(EventHandler::MouseButton button, int x, int y)
{
	if (hidden() || (button != EventHandler::BUTTON_LEFT)) { return; }

	if(!mLeftButtonDown && !mDragging)
		return;


	mDragging = false;
	mLeftButtonDown = !(button == EventHandler::BUTTON_LEFT);
}


/**
 * Determine a pattern using two selected points.
 */
void TilePalette::reset()
{}
