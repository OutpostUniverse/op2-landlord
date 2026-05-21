#include "TilePalette.h"

#include "Common.h"

#include <NAS2D/EnumMouseButton.h>


using namespace NAS2D;


const auto PALETTE_DIMENSIONS = Vector{196, 300};


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
	const auto size = Utility<Renderer>::get().size();
	const auto startPoint = NAS2D::Point{-2, -2} + size - PALETTE_DIMENSIONS;
	_rect() = NAS2D::Rectangle(startPoint, PALETTE_DIMENSIONS);

	text("Tile Palette");
}


/**
 * Draws the tile palette
 */
void TilePalette::draw()
{
}


void TilePalette::mouseDown(MouseButton button, NAS2D::Point<int> position)
{
	if (!visible() || (button != MouseButton::Left)) { return; }

	if (!(rect().contains(_mouseCoords())))
	{
		return;
	}

	mLeftButtonDown = true;
}


void TilePalette::mouseUp(MouseButton button, NAS2D::Point<int> position)
{
	mLeftButtonDown = false;

	if (!visible() || (button != MouseButton::Left)) { return; }
	if (!mLeftButtonDown && !dragging()) { return; }
}


/**
 * Determine a pattern using two selected points.
 */
void TilePalette::reset()
{}
