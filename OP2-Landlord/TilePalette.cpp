#include "TilePalette.h"

#include "Common.h"


const auto PALETTE_DIMENSIONS = Vector{196, 300};


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
