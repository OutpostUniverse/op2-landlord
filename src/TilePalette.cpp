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
TilePalette::TilePalette()
{
	_init();
}


TilePalette::~TilePalette()
{}

void TilePalette::_init()
{
	int yPosition = rect().y() + rect().height() - 23;

	Renderer& r = Utility<Renderer>::get();
	_rect()(r.width() - PALETTE_DIMENSIONS.x() - 2, r.height() - PALETTE_DIMENSIONS.y() - 2, PALETTE_DIMENSIONS.x(), PALETTE_DIMENSIONS.y());

	text("Tile Palette");
}


/**
 * Draws the tile palette
 */
void TilePalette::draw()
{
	Renderer& r = Utility<Renderer>::get();

}


void TilePalette::mouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!visible() || (button != EventHandler::BUTTON_LEFT)) { return; }

	if (!(isPointInRect(_mouseCoords(), rect())))
	{
		return;
	}

	mLeftButtonDown = true;
}


void TilePalette::mouseUp(EventHandler::MouseButton button, int x, int y)
{
	mLeftButtonDown = false;

	if (!visible() || (button != EventHandler::BUTTON_LEFT)) { return; }
	if (!mLeftButtonDown && !dragging()) { return; }
}


/**
 * Determine a pattern using two selected points.
 */
void TilePalette::reset()
{}
