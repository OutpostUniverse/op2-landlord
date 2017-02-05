#ifndef __NAS_CELL__
#define __NAS_CELL__

#include <string>

#include "NAS2D/NAS2D.h"

using namespace NAS2D;

/**
 * 
 */
class Cell
{
public:

	enum TileLayer
	{
		LAYER_BASE,
		LAYER_BASE_DETAIL,
		LAYER_DETAIL,
		LAYER_FOREGROUND
	};

	static const int EMPTY_INDEX = -1;

	Cell();
	Cell(int baseIndex, int baseDetailIndex, int detailIndex, int fgIndex, bool blocked);

	int index(TileLayer layer = LAYER_BASE) const;
	void index(TileLayer layer, int index);

	bool blocked() const;
	void blocked(bool blocked);

	bool linked() const { return !mLink.empty(); }

	Point_2d link_destination() const { return Point_2d(mLinkX, mLinkY); }
	void link_destination(const Point_2d& pt) { mLinkX = pt.x(); mLinkY = pt.y(); }

	const std::string& link() const { return mLink; };
	void link(const std::string& link) { mLink = link; }

private:

	std::string		mLink;				/**< Map to link to. */

	int				mLinkX;				/**<  */
	int				mLinkY;				/**<  */

	int				mBaseIndex;			/**< Tile index for the Sub-Background tile. */
	int				mBaseDetailIndex;	/**< Tile index for the Background tile */
	int				mDetailIndex;		/**< Tile index for the Detail tile. */
	int				mFgIndex;			/**< Tile index for the Foreground index. */

	bool			mBlocked;			/**< Indicates that this tile can't be walked on. */
};


#endif