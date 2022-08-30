#include "TilePalette.h"

#include "Common.h"


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
	_rect() = NAS2D::Rectangle<int>::Create(startPoint, PALETTE_DIMENSIONS);

	text("Tile Palette");
}


/**
 * Draws the tile palette
 */
void TilePalette::draw()
{
}


void TilePalette::mouseDown(EventHandler::MouseButton button, NAS2D::Point<int> position)
{
	if (!visible() || (button != EventHandler::MouseButton::Left)) { return; }

	if (!(rect().contains(_mouseCoords())))
	{
		return;
	}

	mLeftButtonDown = true;
}


void TilePalette::mouseUp(EventHandler::MouseButton button, NAS2D::Point<int> position)
{
	mLeftButtonDown = false;

	if (!visible() || (button != EventHandler::MouseButton::Left)) { return; }
	if (!mLeftButtonDown && !dragging()) { return; }
}


/**
 * Determine a pattern using two selected points.
 */
void TilePalette::reset()
{}
