#include "Cell.h"


/**
 * C'tor
 */
Cell::Cell():	mLinkX(0),
				mLinkY(0),
				mBaseIndex(0),
				mBaseDetailIndex(EMPTY_INDEX),
				mDetailIndex(EMPTY_INDEX),
				mFgIndex(EMPTY_INDEX),
				mBlocked(false)
{}


/**
 * C'tor
 */
Cell::Cell(int baseIndex, int baseDetailIndex, int detailIndex, int fgIndex, bool blocked):		mLinkX(0),
																								mLinkY(0),
																								mBaseIndex(baseIndex),
																								mBaseDetailIndex(baseDetailIndex),
																								mDetailIndex(detailIndex),
																								mFgIndex(fgIndex),
																								mBlocked(blocked)
{}


/**
 * 
 */
int Cell::index(TileLayer layer) const
{
	switch(layer)
	{
		case LAYER_BASE:
			return mBaseIndex;
			break;

		case LAYER_BASE_DETAIL:
			return mBaseDetailIndex;
			break;

		case LAYER_DETAIL:
			return mDetailIndex;
			break;

		case LAYER_FOREGROUND:
			return mFgIndex;
			break;

		default:
			return 0;
			break;
	}
}


/**
 * 
 */
void Cell::index(TileLayer layer, int index)
{
	switch(layer)
	{
		case LAYER_BASE:
			mBaseIndex = index;
			break;

		case LAYER_BASE_DETAIL:
			mBaseDetailIndex = index;
			break;

		case LAYER_DETAIL:
			mDetailIndex = index;
			break;

		case LAYER_FOREGROUND:
			mFgIndex = index;
			break;

		default:
			break;
	}
}


/**
 * 
 */
bool Cell::blocked() const
{
	return mBlocked;
}


/**
 * 
 */
void Cell::blocked(bool blocked)
{
	mBlocked = blocked;
}