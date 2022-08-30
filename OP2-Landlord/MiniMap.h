#pragma once

#include "Window.h"

#include "Map/MapFile.h"

#include "NAS2D/NAS2D.h"


class MiniMap : public Window
{
public:

	MiniMap();
	virtual ~MiniMap();

	bool moving_camera() const { return mMovingCamera; }

	void map(MapFile* map) { mMap = map; update_minimap(); }

	void update_minimap();

private:
	void _init();

	virtual void draw();

	void mouseDown(NAS2D::EventHandler::MouseButton b, NAS2D::Point<int> position);
	void mouseUp(NAS2D::EventHandler::MouseButton b, NAS2D::Point<int> position);
	void mouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change);

	void createMiniMap();

	void adjustCamera(int x, int y);

private:
	NAS2D::Rectangle<int>	mViewRect;

	NAS2D::Image*			mMiniMap = nullptr;

	MapFile*		mMap = nullptr;

	bool			mLeftButtonDown = false;
	bool			mMovingCamera = false;
};