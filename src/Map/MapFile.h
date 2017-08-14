#pragma once

#include "cell_types.h"
#include "TileGroup.h"
#include "TileSetManager.h"


#include "../Common.h"


/**
 * 
 */
class MapFile
{
public:
	// Class specific
	MapFile(const std::string& filename);
	MapFile(const std::string& filename, BaseTerrain base, int width, int height);
	~MapFile();

	int width();
	int height();

	int tileGroupCount() const;
	const NAS2D::Point_2d& tileGroupExtents() const;

	TileGroup* tileGroup(int tg_index);
	const std::string& tileGroupName(int tg_index);

	const NAS2D::Color_4ub& tile_color(int x, int y);

	int index(int x, int y);
	void index(int x, int y, int index);

	CellType cellType(int x, int y) const;
	void cellType(int x, int y, CellType type);

	bool lavaPossible(int x, int y);
	void lavaPossible(int x, int y, int lavaPossible);

	bool aroundTheWorld() const;

	const NAS2D::Point_2d& cameraPosition() const { return mCameraPosition; }
	void updateCameraAnchorArea(int width, int height);
	void moveCamera(int x, int y);
	void setCamera(int x, int y);

	void draw(int x, int y, int width, int height, bool draw_overlay = false);

private:
	#pragma pack(push, 1) // Make sure structures are byte aligned
	struct MapHeader
	{
		int tag;
		int unknown;
		int lgTileWidth;
		int tileHeight;
		int numTileSets;
	};

	struct ClippingRect
	{
		int x1 = 0;			// Left edge of map (x coordinate)
		int y1 = 0;			// Top edge of map (y coordinate)
		int x2 = 0;			// Rigth edge of map (x coordinate)
		int y2 = 0;			// Bottom edge of map (y coordinate)
	};
	#pragma pack(pop)

	struct TileGroupDescriptor
	{
		TileGroupDescriptor(const std::string _name, TileGroup* tg) : name(_name), tilegroup(tg) {}

		std::string name;
		TileGroup* tilegroup = nullptr;
	};

	typedef std::vector<TileGroupDescriptor> TileGroupInfoTable;


private:
	void load(const std::string& filename);
	void save(const std::string& filename);

	void initMapHeader();
	void validateCoords(int x, int y) const;

	int tset_index(int x, int y) const;

	int tile_offset(int x, int y) const;

private:
	typedef std::vector<TileSet*> TileSetList;

private:
	// Feature extention values -- these are never saved to the map file.
	TileSetManager*		mTilesetManager = nullptr;		/**<  */

	NAS2D::Point_2d		mLargestTileGroupExtents;		/**< Largest width and height in tiles of all the TileGroups. */

	NAS2D::Point_2d		mCameraPosition;				/**< Current position of the camera. */
	NAS2D::Rectangle_2d	mCameraAnchorArea;				/**< Area that the camera is allowed to move around in. */

	void _readTileGroupName(StreamReader& in, TileGroup& tilegroup);

private:
	// These values are saved to the map file.
	MapHeader			mMapHeadInfo;					/**<  */
	ClippingRect		mClipRect;						/**<  */

	int					mTileWidth = 0;					/**< Cached width value (header contains lgTileWidth) */
	int					mTileHeight = 0;				/**<  */
	int*				mTileData = nullptr;			/**< Tile data array */

	TileSetList			mTileSets;						/**< Source of all tile sets that need to be loaded */

	int					mTileGroupCount = 0;			/**< Number of tile groups stored in map file */

	TileGroupInfoTable	mTileGroups;
};
