#pragma once

#include "NAS2D/NAS2D.h"

#include "Button.h"
#include "Pattern.h"
#include "Window.h"

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
class TilePalette : public Window
{
public:
	TilePalette();
	~TilePalette();

	void reset();

private:
	virtual void draw() final;

	void _init();

	void mouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);
	void mouseUp(NAS2D::EventHandler::MouseButton button, int x, int y);
	void mouseMove(int x, int y, int relX, int relY);

private:
	Point<int>		mStartIndex;				/**< Index of the selected tile(s). */
	Point<int>		mEndIndex;					/**< Index of the selected tile(s). */

	bool			mLeftButtonDown = false;	/**< Flag indicating that the left mouse button is depressed. */
	bool			mMouseOverTiles = false;	/**< Flag indicating that the mouse is actually over the tiles. */
};
