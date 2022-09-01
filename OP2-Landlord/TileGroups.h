#pragma once

#include "NAS2D/NAS2D.h"

#include "Button.h"
#include "Pattern.h"
#include "Slider.h"
#include "Window.h"

#include "Map/MapFile.h"
#include "Map/TileGroup.h"


/**
 * \class TileGroups
 * \brief Implements a basic tile palette window.
 */
class TileGroups : public Window
{
public:
	TileGroups();
	~TileGroups();

	void reset();

	void map(MapFile* map);

private:
	virtual void draw() final;

	void _init();

	void mouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position);
	void mouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position);
	void mouseWheel(NAS2D::Vector<int> change);

	void onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat);

	virtual void positionChanged(float dX, float dY);

	void mSlider_Changed(float);

private:
	int			mTileGroupIndex = 0;		/**<  */

	MapFile*	mMap = nullptr;

	Slider		mSlider;

	bool		mLeftButtonDown = false;	/**< Flag indicating that the left mouse button is depressed. */
};
