#pragma once

#include "TileSet.h"

#include "../Stream/StreamReader.h"
#include "../Stream/StreamWriter.h"

#include <string>

class TileSetManager
{
public:
	TileSetManager(const std::string& tilesetName);
	TileSetManager(int numTileSets, StreamReader *inStream);
	~TileSetManager();

	int tileSets();

	const std::string& tileSetName(int index);

	TileSet* tileSet(int index);

	/** Returns index of the added tileset. */
	int addTileSet(const std::string& tileSetName);

	/** Returns index of the removed tileset. */
	int removeTileSet(const std::string& tileSetName);


	int MapInTiles(int tileSetIndex, int startTile, int numTiles /* [retval][out] int* mappingIndex*/);

	int getMappingIndex(int tileSetIndex, int tileIndex, int numTileReplacements, int cycleDelay);

	int get_NumMappings();

	int get_TileSetIndex(int mappingIndex);
	void put_TileSetIndex(int mappingIndex, int tileSetIndex);

	int get_TileSetTileIndex(int mappingIndex);
	void put_TileSetTileIndex(int mappingIndex, int tileSetTileIndex);

	int get_NumTileReplacements(int mappingIndex);
	void put_NumTileReplacements(int mappingIndex, int numTileReplacements);

	int get_CycleDelay(int mappingIndex);
	void put_CycleDelay(int mappingIndex, int cycleDelay);

	int get_NumTerrains();
	void set_NumTerrains(int numTerrains);

	int get_TerrainStartTile(int terrainIndex);
	void put_TerrainStartTile(int terrainIndex, int startMapping);

	int get_TerrainEndTile(int terrainIndex);
	void put_TerrainEndTile(int terrainIndex, int endMapping);

private:
	friend class MapFile;
	friend class TileGroup;

	#pragma pack(push, 1)

	/**
	 * 
	 */
	struct TileSetInfo
	{
		int numTiles;				// Number of tiles in this tile set
		int stringLength;			// Length of the tileSetName string
		char *tileSetName;			// File name of the tile set, as Outpost2 sees it
		std::string wideTileSetName;// BSTR copy of the tile set name (cached for use with COM)
		TileSet *tileSet;			// A pointer to a tile set object
	};


	/**
	 * 
	 */
	struct TileSetTileMapping
	{
		short tileSetIndex;
		short tileIndex;
		short numTileReplacements;
		short cycleDelay;
	};


	/**
	 * Holds sets of related tiles
	 * 
	 * LavaWalls/MicrobeWalls/NormalWalls/DamagedWalls/ReallyDamagedWalls/Tubes
	 */
	struct TerrainTypeItemTable
	{
		short tile[0x10];		// tile for each direction facing
	};

	
	/**
	 * 
	 */
	struct TerrainType
	{
		short firstTile;		// First tile index in this terrain type class
		short lastTile;			// Last tile index in this terrain type class
		short bulldozed;		// Index of the bulldozed tile
		short commonRubble;		// Index of the common rubble tile (4 common rubble tiles, followed by 4 rare rubble tiles)
		short tubeUnknown[6];	// **TODO** find out use of this (data is repeated below)
		TerrainTypeItemTable wall[5];	// Wall groups
		short lava;
		short flat1;
		short flat2;
		short flat3;
		TerrainTypeItemTable tube;		// Tube group
		short scorched;			// Scorched tile index (from vehicle explosion)

		short unknown[0x15];	// **TODO** find out
	};
	#pragma pack(pop)

private:
	int Save(StreamWriter *stream);
	int Load(StreamReader *stream);
	void FreeMemory();

	void validateIndex(int index);
	void validateMappingIndex(int index);
	void validateTerrainIndex(int index);

private:
	int					numTileSets = 512;			/**< Number of loaded tile sets */
	int					mNumMappings = 0;			/**< Number of tile mappings in the tile mapping array */
	int					mTerrainCount = 0;			/**< Number of terrain types in the terrain type array */

	TileSetInfo*		mTileSetInfo = nullptr;		/**< List of loaded tile sets */
	TileSetTileMapping*	mMapping = nullptr;			/**< Tile set tile mapping array */
	TerrainType*		mTerrain = nullptr;			/**< Tarrain type array */
};
