#include "TilePalette.h"

#include "ConfigStrings.h"

#include "Common.h"

const Point_2d		PALETTE_DIMENSIONS		= Point_2d(196, 300);

const Point_2d		FIRST_TILE_COORDINATE	= Point_2d(2, 18);
const Point_2d		TILE_SLOT_PADDING		= Point_2d(0, 0);

const Point_2d		TILE_GRID_DIMENSIONS	= Point_2d(6, 8);

const int			NUM_TILES_PER_PAGE		= TILE_GRID_DIMENSIONS.x() * TILE_GRID_DIMENSIONS.y();

/**
 * C'Tor
 */
TilePalette::TilePalette():		mFont(nullptr),
								mTset(nullptr),
								mRect(Utility<Renderer>::get().width() - PALETTE_DIMENSIONS.x()- 2, Utility<Renderer>::get().height() - PALETTE_DIMENSIONS.y() - 2, PALETTE_DIMENSIONS.x(), PALETTE_DIMENSIONS.y()),
								mNumPages(0),
								mCurrentPage(0),
								mTsetIndex(0),
								mHidden(false),
								mLeftButtonDown(false),
								mMouseOverTiles(false),
								mDragging(false)
{
	init();
}


TilePalette::~TilePalette()
{
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().Disconnect(this, &TilePalette::onMouseDown);
	e.mouseButtonUp().Disconnect(this, &TilePalette::onMouseUp);
	e.mouseMotion().Disconnect(this, &TilePalette::onMouseMove);
}

void TilePalette::init()
{
	int yPosition = rect().y() + rect().h() - 23;

	mBtnPrev.size(30, 20);
	mBtnPrev.position(mRect.x() + 3, yPosition);
	mBtnPrev.image("sys/prev.png");
	mBtnPrev.click().Connect(this, &TilePalette::button_Prev_click);

	mBtnNext.size(30, 20);
	mBtnNext.position(mRect.x() + mRect.w() - (mBtnNext.width() + 2), yPosition);
	mBtnNext.image("sys/next.png");
	mBtnNext.click().Connect(this, &TilePalette::button_Next_click);

	// Hook events
	EventHandler& e = Utility<EventHandler>::get();
	e.mouseButtonDown().Connect(this, &TilePalette::onMouseDown);
	e.mouseButtonUp().Connect(this, &TilePalette::onMouseUp);
	e.mouseMotion().Connect(this, &TilePalette::onMouseMove);
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
			mSlots[row].push_back(Rectangle_2d(FIRST_TILE_COORDINATE.x() + (col * mTset->width()) + (col * TILE_SLOT_PADDING.x()) + mRect.x(), FIRST_TILE_COORDINATE.y() + (row * mTset->height()) + (row * TILE_SLOT_PADDING.y()) + rect().y(), mTset->width(), mTset->height()));
		}
	}

	mTileGridRect(mRect.x() + FIRST_TILE_COORDINATE.x(), mRect.y() + FIRST_TILE_COORDINATE.y(), TILE_GRID_DIMENSIONS.x() * mTset->width(), TILE_GRID_DIMENSIONS.y() * mTset->height());
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
}


/**
 * Draws the tile palette
 */
void TilePalette::update()
{
	if (hidden())
		return;

	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), 180, 180, 180);
	r.drawBoxFilled(mTileGridRect, 70, 70, 70);
	r.drawBoxFilled(rect().x(), rect().y(), rect().w(), 16, 75, 95, 130);
	r.drawBox(rect(), 0, 0, 0);

	if (mFont)
	{
		r.drawText(*mFont, "Tile Palette", mRect.x() + (mRect.w() / 2) - (mFont->width("Tile Palette") / 2), mRect.y() + 4, 255, 255, 255);
		r.drawText(*mFont, "Tile Palette", mRect.x() + (mRect.w() / 2) - (mFont->width("Tile Palette") / 2) + 1, mRect.y() + 4, 255, 255, 255); // cheap way of getting a 'bold' typeface. Inefficient.
		r.drawText(*mFont, string_format("Page: %i of %i", mCurrentPage + 1, mNumPages), mBtnPrev.positionX() + mBtnPrev.width() + 4, mRect.y() + 280, 0, 0, 0);
	}


	if(!mTset)
		return;

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

			r.drawBox(mSlots[row][col], 0, 0, 0);
		}
	}

	// Draw Selectors
	if(mTsetIndex / NUM_TILES_PER_PAGE == mCurrentPage)
	{
		Rectangle_2d rect = mSlots[mStartIndex.y()][mStartIndex.x()];

		r.drawBoxFilled(rect.x(), rect.y(), (mBrushPattern.width() * mTset->width()), (mBrushPattern.height() * mTset->height()), 255, 255, 0, 50);
		r.drawBox(rect.x(), rect.y(), (mBrushPattern.width() * mTset->width()), (mBrushPattern.height() * mTset->height()), 255, 255, 0);
	}

	if(mMouseOverTiles && responding_to_events())
	{
		Rectangle_2d rect = getRectFromPoints(mMouseCoords, mDragOrigin);
		r.drawBox(rect, 255, 255, 255);
	}
}


void TilePalette::onMouseMove(int x, int y, int relX, int relY)
{
	mMouseCoords(x, y);

	if(hidden())
		return;

	if (mDragging)
	{
		mRect.x(mRect.x() + relX);
		mRect.y(mRect.y() + relY);

		mBtnPrev.position(mBtnPrev.positionX() + relX, mBtnPrev.positionY() + relY);
		mBtnNext.position(mBtnNext.positionX() + relX, mBtnNext.positionY() + relY);

		mTileGridRect.x(mTileGridRect.x() + relX);
		mTileGridRect.y(mTileGridRect.y() + relY);

		for (size_t row = 0; row < mSlots.size(); ++row)
		{
			for (size_t col = 0; col < mSlots[row].size(); ++col)
			{
				mSlots[row][col].x(mSlots[row][col].x() + relX);
				mSlots[row][col].y(mSlots[row][col].y() + relY);
			}
		}

		return;
	}

	if(isPointInRect(mMouseCoords, mTileGridRect))
		mMouseOverTiles = true;
	else
		mMouseOverTiles = false;
}


void TilePalette::onMouseDown(MouseButton button, int x, int y)
{
	if(hidden() || (button != BUTTON_LEFT))
		return;

	if (isPointInRect(x, y, rect().x(), rect().y(), rect().w(), 17))
	{
		mDragging = true;
		return;
	}

	if (!(isPointInRect(x, y, rect().x(), rect().y(), rect().w(), rect().h())))
		return;

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
	if (hidden() || mSlots.empty() || (button != BUTTON_LEFT))
		return;

	if(!mLeftButtonDown && !mDragging)
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

	mDragging = false;
	mLeftButtonDown = !(button == BUTTON_LEFT);
}


/**
 * Determine a pattern using two selected points.
 */
void TilePalette::reset()
{
	mTsetIndex = 0;
	mCurrentPage = 0;

	mBrushPattern.size(1, 1);
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