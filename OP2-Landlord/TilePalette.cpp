#include "TilePalette.h"

#include "Common.h"

const Point<int>		PALETTE_DIMENSIONS		= Point<int>(196, 300);

const Point<int>		FIRST_TILE_COORDINATE	= Point<int>(2, 18);
const Point<int>		TILE_SLOT_PADDING		= Point<int>(0, 0);

const Point<int>		TILE_GRID_DIMENSIONS	= Point<int>(6, 8);

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
	Renderer& r = Utility<Renderer>::get();
	_rect() = {r.width() - PALETTE_DIMENSIONS.x() - 2, r.height() - PALETTE_DIMENSIONS.y() - 2, static_cast<float>(PALETTE_DIMENSIONS.x()), static_cast<float>(PALETTE_DIMENSIONS.y())};

	text("Tile Palette");
}


/**
 * Draws the tile palette
 */
void TilePalette::draw()
{
}


void TilePalette::mouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!visible() || (button != EventHandler::MouseButton::BUTTON_LEFT)) { return; }

	if (!(rect().contains(_mouseCoords())))
	{
		return;
	}

	mLeftButtonDown = true;
}


void TilePalette::mouseUp(EventHandler::MouseButton button, int x, int y)
{
	mLeftButtonDown = false;

	if (!visible() || (button != EventHandler::MouseButton::BUTTON_LEFT)) { return; }
	if (!mLeftButtonDown && !dragging()) { return; }
}


/**
 * Determine a pattern using two selected points.
 */
void TilePalette::reset()
{}
