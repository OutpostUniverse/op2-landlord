#pragma once

#include "Window.h"

#include "Map/MapFile.h"

#include "NAS2D/NAS2D.h"


using namespace NAS2D;


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

	void mouseDown(NAS2D::EventHandler::MouseButton b, int x, int y);
	void mouseUp(NAS2D::EventHandler::MouseButton b, int x, int y);
	void mouseMotion(int x, int y, int relX, int relY);

	void createMiniMap();

	void adjustCamera(int x, int y);

private:
	Rectangle<int>	mViewRect;

	Image*			mMiniMap = nullptr;

	MapFile*		mMap = nullptr;

	bool			mLeftButtonDown = false;
	bool			mMovingCamera = false;
};