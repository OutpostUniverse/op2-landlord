#include "TilePalette.h"

#include "ConfigStrings.h"

#include "Common.h"

const Point_2d		PALETTE_DIMENSIONS		= Point_2d(196, 320);

const Point_2d		FIRST_TILE_COORDINATE	= Point_2d(2, 18);
const Point_2d		TILE_SLOT_PADDING		= Point_2d(0, 0);

const Point_2d		TILE_GRID_DIMENSIONS	= Point_2d(6, 8);

const Point_2d		CLOSE_BOX_DIMENSIONS	= Point_2d(12, 12);

const int			NUM_TILES_PER_PAGE		= TILE_GRID_DIMENSIONS.x() * TILE_GRID_DIMENSIONS.y();

const int			HIDE_HEIGHT				= 15;

/**
 * C'Tor
 */
TilePalette::TilePalette():		mFont(nullptr),
								mTset(nullptr),
								mRect(Utility<Renderer>::get().width() - PALETTE_DIMENSIONS.x(), Utility<Renderer>::get().height() - HIDE_HEIGHT, PALETTE_DIMENSIONS.x(), PALETTE_DIMENSIONS.y()),
								mCloseRect(mRect.x() + mRect.w() - (CLOSE_BOX_DIMENSIONS.x() + 2), mRect.y() + 2, CLOSE_BOX_DIMENSIONS.x(), CLOSE_BOX_DIMENSIONS.y()),
								mShow(Utility<Configuration>::get().option(CONFIG_UI_TILEPALETTE_SHOW_IMAGE)),
								mHide(Utility<Configuration>::get().option(CONFIG_UI_TILEPALETTE_DOWN_IMAGE)),
								mNumPages(0),
								mCurrentPage(0),
								mTsetIndex(0),
								mHidden(true),
								mLeftButtonDown(false),
								mMouseOverTiles(false)
{
	int yPosition = Utility<Renderer>::get().height() - PALETTE_DIMENSIONS.y() + 295;

	mBtnPrev.size(30, 20);
	mBtnPrev.position(mRect.x() + 2, yPosition);
	mBtnPrev.image(Utility<Configuration>::get().option(CONFIG_UI_TILEPALETTE_PREV_IMAGE));
	mBtnPrev.click().Connect(this, &TilePalette::button_Prev_click);

	mBtnNext.size(30, 20);
	mBtnNext.position(mRect.x() + mRect.w() - (mBtnNext.width() + 2), yPosition);
	mBtnNext.image(Utility<Configuration>::get().option(CONFIG_UI_TILEPALETTE_NEXT_IMAGE));
	mBtnNext.click().Connect(this, &TilePalette::button_Next_click);
}


void TilePalette::button_Prev_click()
{
	mCurrentPage = clamp(mCurrentPage - 1, 0, mNumPages - 1);
}


void TilePalette::button_Next_click()
{
	mCurrentPage = clamp(mCurrentPage + 1, 0, mNumPages - 1);
}


/**
 * Sets the internal font to use.
 */
void TilePalette::font(Font& font)
{
	mFont = &font;
}


/**
 * Sets the current tileset.
 * 
 * \param	tset	A pointer to a Tileset. May be NULL.
 */
void TilePalette::tileset(Tileset* tset)
{
	mTset = tset;

	if(!tset)
	{
		mNumPages = 0;
		mCurrentPage = 0;
		mHidden = true;
		return;
	}

	mNumPages = divideUp(mTset->numTiles(), NUM_TILES_PER_PAGE);


	mSlots.clear();
	mSlots.resize(TILE_GRID_DIMENSIONS.y());
	for(int row = 0; row < TILE_GRID_DIMENSIONS.y(); row++)
	{
		for(int col = 0; col < TILE_GRID_DIMENSIONS.x(); col++)
		{
			mSlots[row].push_back(Rectangle_2d(FIRST_TILE_COORDINATE.x() + (col * mTset->width()) + (col * TILE_SLOT_PADDING.x()) + mRect.x(), FIRST_TILE_COORDINATE.y() + (row * mTset->height()) + (row * TILE_SLOT_PADDING.y()) + Utility<Renderer>::get().height() - PALETTE_DIMENSIONS.y(), mTset->width(), mTset->height()));
		}
	}

	mTileGridRect = Rectangle_2d(mRect.x() + FIRST_TILE_COORDINATE.x(), mRect.y() + FIRST_TILE_COORDINATE.y(), TILE_GRID_DIMENSIONS.x() * mTset->width(), TILE_GRID_DIMENSIONS.y() * mTset->height());
}


/**
 * Hides or unhides the TilePalette.
 * 
 * \note	If the Tileset pointer is NULL, TilePalette will assume a
 *			non-functional state and will refuse to unhide itself.
 */
void TilePalette::hidden(bool hidden)
{
	if(!mTset)
		return;

	mHidden = hidden;

	Renderer& r = Utility<Renderer>::get();

	// Adjust TilePalette area rect depending on whether or not it's hidden.
	if(mHidden)
		mRect = Rectangle_2d(r.width() - PALETTE_DIMENSIONS.x(), r.height() - HIDE_HEIGHT, PALETTE_DIMENSIONS.x(), PALETTE_DIMENSIONS.y());
	else
		mRect = Rectangle_2d(r.width() - PALETTE_DIMENSIONS.x(), r.height() - PALETTE_DIMENSIONS.y(), PALETTE_DIMENSIONS.x(), PALETTE_DIMENSIONS.y());

	mCloseRect = Rectangle_2d(mRect.x() + mRect.w() - (CLOSE_BOX_DIMENSIONS.x() + 2), mRect.y() + 2, CLOSE_BOX_DIMENSIONS.x(), CLOSE_BOX_DIMENSIONS.y());
	mTileGridRect = Rectangle_2d(mRect.x() + FIRST_TILE_COORDINATE.x(), mRect.y() + FIRST_TILE_COORDINATE.y(), TILE_GRID_DIMENSIONS.x() * mTset->width(), TILE_GRID_DIMENSIONS.y() * mTset->height());
}


/**
 * Draws the tile palette
 */
void TilePalette::update()
{
	Renderer& r = Utility<Renderer>::get();

	if(mHidden || !mTset)
	{
		r.drawBox(mRect.x(), mRect.y(), mRect.w(), mRect.h(), 0, 0, 0, 100);
		r.drawBoxFilled(mRect.x(), mRect.y(), mRect.w(), mRect.h(), 200, 200, 0, 65);
		r.drawImage(mShow, mRect.x() + mRect.w() - mShow.width() - 1, mRect.y() + 1);

		if(mFont)
			r.drawTextShadow(*mFont, "Tile Palette", mRect.x() + 2, mRect.y(), 1, 255, 255, 255, 0, 0, 0);

		return;
	}

	// Draw the background box.
	r.drawBox(mRect, 0, 0, 0, 100);
	r.drawBoxFilled(mRect.x(), mRect.y(), mRect.w(), mRect.h(), 200, 200, 0, 65);

	// Draw the close button
	r.drawImage(mHide, mRect.x() + mRect.w() - mHide.width() - 1, mRect.y() + 1);

	// Draw buttons
	mBtnPrev.update();
	mBtnNext.update();

	// Draw the tile page
	for(int row = 0; row < TILE_GRID_DIMENSIONS.y(); row++)
	{
		for(int col = 0; col < TILE_GRID_DIMENSIONS.x(); col++)
		{
			int index = (row * TILE_GRID_DIMENSIONS.x() + col) + (mCurrentPage * NUM_TILES_PER_PAGE);

			if(index < mTset->numTiles())
				mTset->drawTile(index, FIRST_TILE_COORDINATE.x() + (col * mTset->width()) + (col * TILE_SLOT_PADDING.x()) + mRect.x(), FIRST_TILE_COORDINATE.y() + (row * mTset->height()) + (row * TILE_SLOT_PADDING.y()) + mRect.y());

			//r.drawBox(mSlots[(row * TILE_GRID_DIMENSIONS.x()) + col], 0, 0, 0);
			r.drawBox(mSlots[row][col], 0, 0, 0);
		}
	}

	if(mFont)
	{
		r.drawTextShadow(*mFont, "Tile Palette", mRect.x() + 2, mRect.y() + 2, 1, 255, 255, 255, 0, 0, 0);
		r.drawTextShadow(*mFont, string_format("Page: %i of %i", mCurrentPage + 1, mNumPages), mRect.x() + 1, mRect.y() + 275, 1, 255, 255, 255, 0, 0, 0);
	}

	// Draw Selectors
	if(mTsetIndex / NUM_TILES_PER_PAGE == mCurrentPage)
	{
		Rectangle_2d rect = mSlots[mStartIndex.y()][mStartIndex.x()];

		r.drawBoxFilled(rect.x(), rect.y(), (mBrushPattern.width() * mTset->width()), (mBrushPattern.height() * mTset->height()), 255, 255, 0, 50);
		r.drawBox(rect.x(), rect.y(), (mBrushPattern.width() * mTset->width()), (mBrushPattern.height() * mTset->height()), 255, 255, 0);
	}

	if(mLeftButtonDown && mMouseOverTiles)
	{
		Rectangle_2d rect = getRectFromPoints(mMouseCoords, mDragOrigin);
		r.drawBox(rect, 255, 255, 255);
	}
}


void TilePalette::onMouseMove(int x, int y, int relX, int relY)
{
	if(hidden())
		return;

	mMouseCoords(x, y);

	if(isPointInRect(mMouseCoords, mTileGridRect))
		mMouseOverTiles = true;
	else
		mMouseOverTiles = false;
}


void TilePalette::onMouseDown(MouseButton button, int x, int y)
{
	if(hidden())
		return;

	// ????? Will always be true regardless of which button is actually pushed.
	mLeftButtonDown = true;

	Point_2d pt(x, y);
	mDragOrigin = pt;

	for(int row = 0; row < TILE_GRID_DIMENSIONS.y(); row++)
	{
		for(int col = 0; col < TILE_GRID_DIMENSIONS.x(); col++)
		{
			if(isPointInRect(pt, mSlots[row][col]))
			{
				// Ignore if tile index is out of range.
				int newIndex = (mCurrentPage * NUM_TILES_PER_PAGE) + (TILE_GRID_DIMENSIONS.x() * row) + col;
				if(newIndex < mTset->numTiles())
				{
					mStartIndex.x() = col;
					mStartIndex.y() = row;
					mEndIndex = mStartIndex;

					mTsetIndex = newIndex;
					// Reset the brush pattern.
					mBrushPattern.size(1, 1);
				}

				break;
			}
		}
	}
}


void TilePalette::onMouseUp(MouseButton button, int x, int y)
{
	if(hidden())
		return;

	Point_2d pt(x, y);

	for(int row = 0; row < TILE_GRID_DIMENSIONS.y(); row++)
	{
		for(int col = 0; col < TILE_GRID_DIMENSIONS.x(); col++)
		{
			if(isPointInRect(pt, mSlots[row][col]))
			{
				// Ignore if tile index is out of range.
				int newIndex = (mCurrentPage * NUM_TILES_PER_PAGE) + (TILE_GRID_DIMENSIONS.x() * row) + col;
				if(newIndex < mTset->numTiles())
				{
					mEndIndex.x() = col;
					mEndIndex.y() = row;
					determinePattern();
					break;
				}
			}
		}
	}

	mLeftButtonDown = false;
}


/**
 * Determine a pattern using two selected points.
 */
void TilePalette::determinePattern()
{
	Rectangle_2d rect = getRectFromPoints(mStartIndex, mEndIndex);

	mTsetIndex = (mCurrentPage * NUM_TILES_PER_PAGE) + (TILE_GRID_DIMENSIONS.x() * rect.y()) + rect.x();

	// Set start and end positions.
	mStartIndex.x() = rect.x();
	mStartIndex.y() = rect.y();

	mEndIndex.x() = rect.x() + rect.w() - 1;
	mEndIndex.y() = rect.y() + rect.h() - 1;

	mBrushPattern.size(rect.w(), rect.h());

	for(int row = 0; row < rect.h(); row++)
		for(int col = 0; col < rect.w(); col++)
			mBrushPattern.value(col, row, ((TILE_GRID_DIMENSIONS.x() * row) + col) + mTsetIndex);
}


Rectangle_2d TilePalette::getRectFromPoints(const Point_2d& pt1, const Point_2d& pt2)
{
	Rectangle_2d rect;

	if(pt1.x() < pt2.x())
	{
		rect.x() = pt1.x();
		rect.w() = pt2.x() - pt1.x() + 1;
	}
	else
	{
		rect.x() = pt2.x();
		rect.w() = pt1.x() - pt2.x() + 1;
	}


	if(pt1.y() < pt2.y())
	{
		rect.y() = pt1.y();
		rect.h() = pt2.y() - pt1.y() + 1;
	}
	else
	{
		rect.y() = pt2.y();
		rect.h() = pt1.y() - pt2.y() + 1;
	}

	return rect;
}