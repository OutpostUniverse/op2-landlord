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
	Utility<EventHandler>::get().keyDown().disconnect(this, &TileGroups::onKeyDown);
	Utility<EventHandler>::get().mouseWheel().disconnect(this, &TileGroups::mouseWheel);
}


void TileGroups::_init()
{
	text("Tile Groups");
	mSlider.change().connect(this, &TileGroups::mSlider_Changed);

	Utility<EventHandler>::get().keyDown().connect(this, &TileGroups::onKeyDown);
	Utility<EventHandler>::get().mouseWheel().connect(this, &TileGroups::mouseWheel);
}


void TileGroups::map(MapFile* map)
{
	if (!map)
	{
		throw std::runtime_error("TileGroups::map(): nullptr passed.");
	}

	Renderer& r = Utility<Renderer>::get();

	mMap = map;
	size((mMap->tileGroupExtents().x * TILE_SIZE) + 10, (mMap->tileGroupExtents().y * TILE_SIZE) + 10 + TILE_SIZE + 7);
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

	r.drawText(font(), mMap->tileGroupName(mTileGroupIndex), positionX() + 10, positionY() + titleBarHeight() + 10, 255, 255, 255);

	mSlider.update();
}


void TileGroups::mouseDown(EventHandler::MouseButton button, int x, int y)
{
	if (!visible() || (button != EventHandler::MouseButton::BUTTON_LEFT)) { return; }

	if (!(rect().contains(_mouseCoords())))
	{
		return;
	}

	mLeftButtonDown = true;
}


void TileGroups::mouseUp(EventHandler::MouseButton button, int x, int y)
{
	mLeftButtonDown = false;

	if (!visible() || (button != EventHandler::MouseButton::BUTTON_LEFT)) { return; }
	if (!mLeftButtonDown && !dragging()) { return; }
}


void TileGroups::mouseWheel(int x, int y)
{
	if (!visible()) { return; }

	if (y < 0)
	{
		mSlider.changeThumbPosition(1.0);
	}
	else if (y > 0)
	{
		mSlider.changeThumbPosition(-1.0);
	}
}


void TileGroups::onKeyDown(EventHandler::KeyCode key, EventHandler::KeyModifier mod, bool repeat)
{
	if (!visible()) { return; }

	switch (key)
	{
	case EventHandler::KeyCode::KEY_LEFT:
		mSlider.changeThumbPosition(-1.0);
		break;

	case EventHandler::KeyCode::KEY_RIGHT:
		mSlider.changeThumbPosition(1.0);
		break;

	default:
		break;
	}
}


/**
 * Determine a pattern using two selected points.
 */
void TileGroups::reset()
{
	mTileGroupIndex = 0;
}
