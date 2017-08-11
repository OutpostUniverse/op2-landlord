#pragma once

#include "TileGroup.h"
#include "TileSetManager.h"

class MapFile
{
public:
	enum MapLoadSaveFormat
	{
		MapOnly = 0,
		TileGroups = 1,
		Units = 2,
		SavedGameHeader = 4
	};

public:
	// Class specific
	MapFile(const std::string& tsetName, int width, int height);
	MapFile(const std::string& mapName, MapLoadSaveFormat loadFlags);
	~MapFile();

	int width();
	int height();

	int tileGroups() const;

	int index(int x, int y);
	void index(int x, int y, int index);

	int cellType(int x, int y) const;
	void cellType(int x, int y, int cell_type);

	bool lavaPossible(int x, int y);
	void lavaPossible(int x, int y, int lavaPossible);

	bool aroundTheWorld() const;

	void draw(int x, int y, int width, int height);

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
	struct TileGroupInfo
	{
		TileGroup* tileGroup;
		int nameLen;
		char* name;
	};

private:
	int LoadMap(const std::string& mapName, int loadFlags);
	int SaveMap(const std::string& filename, enum MapLoadSaveFormat saveFlags);
	void FreeMemory();

	void initMapHeader();
	void validateCoords(int x, int y) const;

	int tset_index(int x, int y);

private:
	typedef std::vector<TileSet*> TileSetList;

private:
	TileSetManager*		mTilesetManager = nullptr;		/**<  */

	MapHeader			mMapHeadInfo;					/**<  */
	ClippingRect		mClipRect;						/**<  */

	NAS2D::Point_2d		mCameraPosition;				/**<  */

	int					mTileWidth = 0;					/**< Cached width value (header contains lgTileWidth) */
	int					mTileHeight = 0;				/**<  */
	int*				mTileData = nullptr;			/**< Tile data array */

	TileSetList			mTileSets;						/**< Source of all tile sets that need to be loaded */

	int					mTileGroupCount = 0;			/**< Number of tile groups stored in map file */
	TileGroupInfo*		mTileGroupInfo = nullptr;		/**< Array of named tile groups */
};
