#pragma once

#include "NAS2D/NAS2D.h"

#include "Map.h"

using namespace NAS2D;


class MiniMap
{
public:

	MiniMap();
	~MiniMap();

	Rectangle_2d& rect() { return mRect; }

	void font(Font* _f) { mFont = _f; }
	void map(Map* _m);

	void hidden(bool _b);
	bool hidden() const { return mHidden; }

	bool dragging() const { return mDragging; }
	bool moving_camera() const { return mMovingCamera; }

	bool responding_to_events() const { return dragging() || moving_camera(); }

	void update();

	void update_minimap();

private:

	void init();

	void onMouseDown(MouseButton b, int x, int y);
	void onMouseUp(MouseButton b, int x, int y);
	void onMouseMotion(int x, int y, int relX, int relY);

	void createMiniMap();

	void adjustCamera(int x, int y);

private:

	Rectangle_2d	mRect;

	Font*			mFont;

	SDL_Surface*	mSurface;
	Image*			mMiniMap;

	Map*			mMap;

	bool			mDragging;
	bool			mLeftButtonDown;
	bool			mMovingCamera;
	bool			mHidden;
};