#pragma once

#include "NAS2D/NAS2D.h"

#include "Button.h"
#include "Pattern.h"


using namespace NAS2D;

/**
 * \class TilePalette
 * \brief Implements a basic tile palette window.
 * 
 * TilePalette assumes that the tileset pointer points to a valid location in
 * memory. The pointer may be NULL in which case the TilePalette will assume
 * a non-functional state (won't come out of hiding.
 * 
 * TilePalette aligns itself to the bottom right corner of the screen regarless
 * of resolution. It can't be moved around -- makes things a lot easier.
 */
class TilePalette
{
public:
	TilePalette();
	~TilePalette();

	void update();

	void hidden(bool hidden);
	bool hidden() const { return mHidden; }

	void font(Font& font);
	void boldFont(Font& font);

	const Rectangle_2d& rect() const { return mRect; }

	void reset();

	bool dragging() const { return mDragging; }

	bool responding_to_events() { return dragging() || mLeftButtonDown; }

private:
	void init();

	void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);
	void onMouseUp(NAS2D::EventHandler::MouseButton button, int x, int y);
	void onMouseMove(int x, int y, int relX, int relY);

private:
	Font*			mFont = nullptr;		/**< Font to use to draw basic information. */
	Font*			mBoldFont = nullptr;		/**< Font to use to draw basic information. */

	Rectangle_2d	mRect;				/**< Area and position of the palette. */

	Point_2d		mStartIndex;		/**< Index of the selected tile(s). */
	Point_2d		mEndIndex;			/**< Index of the selected tile(s). */

	Point_2d		mMouseCoords;		/**< Point where a drag started. */

	bool			mHidden = false;			/**< Flag indicating that the palette shouldn't be drawn. */
	bool			mLeftButtonDown = false;	/**< Flag indicating that the left mouse button is depressed. */
	bool			mMouseOverTiles = false;	/**< Flag indicating that the mouse is actually over the tiles. */
	bool			mDragging = false;			/**< Flag indicating that the tile palette is being dragged from its title bar. */
};
