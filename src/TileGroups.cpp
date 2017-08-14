#include "TileGroups.h"
#include "Common.h"

using namespace NAS2D;

/**
 * C'Tor
 */
TileGroups::TileGroups()
{
	_init();
}


/**
 * D'Tor
 */
TileGroups::~TileGroups()
{
	mSlider.change().disconnect(this, &TileGroups::mSlider_Changed);
}


void TileGroups::_init()
{
	Renderer& r = Utility<Renderer>::get();
	text("Tile Groups");
	mSlider.change().connect(this, &TileGroups::mSlider_Changed);
}


void TileGroups::map(MapFile* map)
{
	if (!map)
	{
		throw std::runtime_error("TileGroups::map(): nullptr passed.");
	}

	Renderer& r = Utility<Renderer>::get();

	mMap = map;
	size((mMap->tileGroupExtents().x() * TILE_SIZE) + 30, (mMap->tileGroupExtents().y() * TILE_SIZE) + 10 + TILE_SIZE);
	position(r.width() - width() - 5, r.height() - height() - 5);

	mSlider.font(font());
	mSlider.position(positionX() + 5, positionY() + height() - 20);
	mSlider.size(width() - 10, 15);
	mSlider.length((double)mMap->tileGroupCount() - 1);
	mSlider.displayPosition(true);
}


void TileGroups::mSlider_Changed(double pos)
{
	mTileGroupIndex = static_cast<int>(pos);
}


/**
 * Slider doesn't play nice with UIContainer so we need
 * to update its position here or it misbehaves.
 * 
 * \todo	Fix slider so that this becomes unnecessary.
 */
void TileGroups::positionChanged(float dX, float dY)
{
	mSlider.position(positionX() + 5, positionY() + height() - 20);
}


/**
 * Draws the tile palette
 */
void TileGroups::draw()
{
	Renderer& r = Utility<Renderer>::get();

	mMap->tileGroup(mTileGroupIndex)->draw(rect().x() + 5, rect().y() + titleBarHeight() + 5);

	mSlider.update();
}


void TileGroups::mouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!visible() || (button != EventHandler::BUTTON_LEFT)) { return; }

	if (!(isPointInRect(_mouseCoords(), rect())))
	{
		return;
	}

	mLeftButtonDown = true;
}


void TileGroups::mouseUp(EventHandler::MouseButton button, int x, int y)
{
	mLeftButtonDown = false;

	if (!visible() || (button != EventHandler::BUTTON_LEFT)) { return; }
	if (!mLeftButtonDown && !dragging()) { return; }
}


/**
 * Determine a pattern using two selected points.
 */
void TileGroups::reset()
{
	mTileGroupIndex = 0;
}
