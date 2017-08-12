#pragma once

#include "Map/MapFile.h"

#include "NAS2D/NAS2D.h"

#include <SDL.h>


using namespace NAS2D;


class MiniMap
{
public:

	MiniMap();
	~MiniMap();

	Rectangle_2d& rect() { return mRect; }

	void font(Font& _f) { mFont = &_f; }
	void boldFont(Font& _f) { mBoldFont = &_f; }

	void hidden(bool _b);
	bool hidden() const { return mHidden; }

	bool dragging() const { return mDragging; }
	bool moving_camera() const { return mMovingCamera; }

	bool responding_to_events() const { return dragging() || moving_camera(); }

	void map(MapFile* map) { mMap = map; update_minimap(); }

	void update();

	void update_minimap();

private:
	void init();

	void onMouseDown(NAS2D::EventHandler::MouseButton b, int x, int y);
	void onMouseUp(NAS2D::EventHandler::MouseButton b, int x, int y);
	void onMouseMotion(int x, int y, int relX, int relY);

	void createMiniMap();

	void adjustCamera(int x, int y);

private:
	Rectangle_2d	mRect;
	Rectangle_2d	mViewRect;

	Font*			mFont = nullptr;
	Font*			mBoldFont = nullptr;

	SDL_Surface*	mSurface = nullptr;
	Image*			mMiniMap = nullptr;

	MapFile*		mMap = nullptr;

	bool			mDragging = false;
	bool			mLeftButtonDown = false;
	bool			mMovingCamera = false;
	bool			mHidden = false;
};