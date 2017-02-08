#ifndef __TILESET__
#define __TILESET__

#include "NAS2D/Resources/Image.h"

#include <string>
#include <vector>

using namespace NAS2D;

/**
 * \class	Tileset
 * \brief	A basic tileset class.
 */
class Tileset
{
public:

	Tileset() {}

	Tileset(const std::string& path, int tileWidth, int tileHeight);

	void drawTile(int index, int x, int y);

	void drawTileToImage(Image& img, int index, int x, int y);

	/**
	 * Gets the width of a Tile.
	 */
	int width() const { return mTileDimensions.x(); }
	
	/**
	 * Gets the height of a Tile.
	 */
	int height() const { return mTileDimensions.y(); }

	int halfWidth() const { return mTileHalfDimensions.x(); }
	int halfHeight() const { return mTileHalfDimensions.y(); }

	int numTiles() const { return mTilesetDimensions.x() * mTilesetDimensions.y(); }

	const std::string& filepath() const { return mTsetPath; }

	const Color_4ub& averageColor(int index);

	void drawTileColorPalette(int x, int y, int cell_size, int columns = 16);

private:
	typedef std::vector<Color_4ub> ColorList;

	void init();
	void fillTileColorList();

	const Rectangle_2d getTsetCoordsFromIndex(int index) const;

	Image		mTileset;

	std::string	mTsetPath;

	Point_2d	mTileDimensions;
	Point_2d	mTileHalfDimensions;
	Point_2d	mTilesetDimensions;

	ColorList	mAverageTileColorsList;
};


#endif