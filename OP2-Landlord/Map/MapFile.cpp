#include "MapFile.h"

#include "cell_types.h"
#include "tilegroup_ignore.h"

#include "../Common.h"
#include "../Stream/StreamReader.h"

#include <iostream>
#include <algorithm>

#include <NAS2D/NAS2D.h>

using namespace NAS2D;


Image* CELL_TYPE_OVERLAY = nullptr;


/**
 * C'tor
 * 
 * Loads an existing map file.
 */
MapFile::MapFile(const std::string& filename)
{
	// Initialize variables

	if(!CELL_TYPE_OVERLAY) { CELL_TYPE_OVERLAY = new Image("sys/celltypemask.png"); }

	load(filename);
}


/**
 * C'tor
 * 
 * Creates a new map from scratch.
 * 
 * \todo	Make this actually do something.
 */
MapFile::MapFile(const std::string& filename, BaseTerrain base, int width, int height)
{
	if (!CELL_TYPE_OVERLAY) { CELL_TYPE_OVERLAY = new Image("sys/celltypemask.png"); }
}


/**
 * D'tor
 */
MapFile::~MapFile()
{
	if (mTileData)
	{
		delete[] mTileData;
	}

	if (mTilesetManager)
	{
		delete mTilesetManager;
	}

	for (auto _tgd : mTileGroups)
	{
		delete _tgd.tilegroup;
	}

	if (CELL_TYPE_OVERLAY) { delete CELL_TYPE_OVERLAY; }
}


/**
 * 
 */
void MapFile::initMapHeader()
{
	// Initialize map header structure
	mMapHeadInfo.tag = 0x00001011;
	mMapHeadInfo.unknown = 0;
	mMapHeadInfo.tileHeight = mTileHeight;
	mMapHeadInfo.numTileSets = 512;

	// Calculate the log (base 2) of the map width
	mMapHeadInfo.lgTileWidth = LogBase2(mTileWidth);
}


/**
 * 
 */
int MapFile::width()
{
	return mTileWidth;
}


/**
 * 
 */
int MapFile::height()
{
	return mTileHeight;
}


/**
 * 
 */
void MapFile::validateCoords(int x, int y) const
{
	// Error check array indicies
	if ((x < 0) || (x >= mTileWidth) || (y < 0) || (y >= mTileHeight))
	{
		throw std::runtime_error("MapFile::Index(): Invalid tile location.");
	}
}


/**
 * 
 */
int MapFile::tile_offset(int x, int y) const
{
	int tileXUpper = x / 32;
	int tileXLower = x % 32;
	return (((tileXUpper * mTileHeight) + y) * 32) + tileXLower;
}


/**
 * 
 */
int MapFile::index(int x, int y)
{
	validateCoords(x, y);

	int tileOffset = tile_offset(x, y);

	return (mTileData[tileOffset] & 0x0000FFE0) >> 5;;
}


/**
 * 
 */
const Color& MapFile::tile_color(int x, int y)
{
	validateCoords(x, y);

	TileSetManager::TileSetTileMapping* tileMap = &mTilesetManager->mMapping[tset_index(x, y)];
	TileSet* tileSet = mTilesetManager->mTileSetInfo[tileMap->tileSetIndex].tileSet;

	return tileSet->color(tileMap->tileIndex);
}


/**
 * 
 */
void MapFile::index(int x, int y, int index)
{
	validateCoords(x, y);

	// Check for invalid mapping index
	if ((index & ~0x7FF) != 0)	// Only 11 bits allowed
	{
		throw std::runtime_error("MapFile::Index(): Invalid tile index.");
	}

	int tileOffset = tile_offset(x, y);

	// Set the tile mapping index
	mTileData[tileOffset] = (mTileData[tileOffset] & 0xFFFF001F) | (index << 5);
}


/**
 * 
 */
int MapFile::tset_index(int x, int y) const
{
	return std::clamp((mTileData[tile_offset(x, y)] & 0x0000FFE0) / TILE_SIZE, 0, mTilesetManager->mNumMappings);
}


/**
 * 
 */
void MapFile::updateCameraAnchorArea(int width, int height)
{
	mCameraAnchorArea = {0, 0, mTileWidth * TILE_SIZE - width, mTileHeight * TILE_SIZE - height};
	setCamera(mCameraPosition.x, mCameraPosition.y);
}


/**
 * 
 */
TileGroup* MapFile::tileGroup(int tg_index)
{
	if (tg_index < 0 || tg_index >= tileGroupCount())
	{
		throw std::runtime_error("MapFile::tileGroup(): invalid index.");
	}

	return mTileGroups[tg_index].tilegroup;
}


/**
 * 
 */
const std::string& MapFile::tileGroupName(int tg_index)
{
	if (tg_index < 0 || tg_index >= tileGroupCount())
	{
		throw std::runtime_error("MapFile::tileGroupName(): invalid index.");
	}

	return mTileGroups[tg_index].name;
}


/**
 * 
 */
void MapFile::moveCamera(int x, int y)
{
	setCamera(mCameraPosition.x + x, mCameraPosition.y + y);
}


/**
 * 
 */
void MapFile::setCamera(int x, int y)
{
	mCameraPosition = {std::clamp(x, 0, mCameraAnchorArea.width), std::clamp(y, 0, mCameraAnchorArea.height)};
}


/**
 * 
 */
void MapFile::draw(int x, int y, int width, int height, bool draw_overlay)
{
	int offsetX = mCameraPosition.x / 32;
	int offsetY = mCameraPosition.y / 32;

	int drawOffsetX = mCameraPosition.x % 32;
	int drawOffsetY = mCameraPosition.y % 32;

	int columns = (width / 32) + 2;
	int rows = (height / 32) + 2;

	int tileIndex = 0;

	Renderer& r = Utility<Renderer>::get();

	int rasterX = 0, rasterY = 0;

	TileSet* tileSet = nullptr;
	TileSetManager::TileSetTileMapping* tileMap = nullptr;
	for (int row = 0; row <= rows && row + offsetY < mTileHeight; ++row)
	{
		for (int col = 0; col <= columns && col + offsetX < mTileWidth; ++col)
		{
			rasterX = x + (col * 32) - drawOffsetX;
			rasterY = y + (row * 32) - drawOffsetY;

			tileIndex = tset_index(col + offsetX, row + offsetY);

			tileMap = &mTilesetManager->mMapping[tileIndex];
			tileSet = mTilesetManager->mTileSetInfo[tileMap->tileSetIndex].tileSet;
			if (tileSet)
			{
				tileSet->draw(tileMap->tileIndex, rasterX, rasterY);
			}

			if (draw_overlay)
			{
				r.drawSubImage(*CELL_TYPE_OVERLAY, NAS2D::Point{rasterX, rasterY}, NAS2D::Rectangle{0, cellType(col + offsetX, row + offsetY) * TILE_SIZE, TILE_SIZE, TILE_SIZE});
			}
		}
	}
}


/**
 * 
 */
bool MapFile::aroundTheWorld() const
{
	return (mMapHeadInfo.lgTileWidth >= 9);
}


/**
 * 
 */
CellType MapFile::cellType(int x, int y) const
{
	validateCoords(x, y);
	return static_cast<CellType>(mTileData[tile_offset(x, y)] & 0x1F);
}


/**
 * 
 */
void MapFile::cellType(int x, int y, CellType type)
{
	validateCoords(x, y);

	int _ctype = static_cast<int>(type);

	// Check for invalid cell type
	if ((_ctype & ~0x1F) != 0)	// Only 5 bits allowed
	{
		throw std::runtime_error("MapFile::Index(): Invalid cell type.");
	}

	int tileXUpper = x >> 5;
	int tileXLower = y & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	mTileData[tileOffset] = (mTileData[tileOffset] & 0xFFFFFFE0) | (_ctype & 0x1F);
}


/**
 * 
 */
bool MapFile::lavaPossible(int x, int y)
{
	validateCoords(x, y);

	int tileXUpper = x >> 5;
	int tileXLower = x & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	return (mTileData[tileOffset] >> 28) & 0x1;;
}


/**
 * 
 */
void MapFile::lavaPossible(int x, int y, int lavaPossible)
{
	validateCoords(x, y);

	int tileXUpper = x >> 5;
	int tileXLower = x & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	mTileData[tileOffset] |= (lavaPossible & 0x1) << 28;
}


/**
 * 
 */
int MapFile::tileGroupCount() const
{
	return mTileGroups.size();
}


/**
 * Gets the largest extents for all TileGroups.
 */
const Point<int>& MapFile::tileGroupExtents() const
{
	return mLargestTileGroupExtents;
}


/**
 * Internal function to read a tile group name out of an input stream.
 */
void MapFile::_readTileGroupName(StreamReader& in, TileGroup& tilegroup)
{
	// Read in the tile group name length
	int name_len = 0;
	in.read(&name_len, MAP_CHUNK_SIZE);

	char* tilegroup_name = new char[name_len + 1];
	in.read(tilegroup_name, name_len);
	tilegroup_name[name_len] = '\0';

	for (auto _tgname : TILEGROUP_IGNORE_LIST)
	{
		if (tilegroup_name == _tgname)
		{
			delete[] tilegroup_name;
			delete (&tilegroup);
			return;
		}
	}

	if (tilegroup.width() > mLargestTileGroupExtents.x) { mLargestTileGroupExtents.x = tilegroup.width(); }
	if (tilegroup.height() > mLargestTileGroupExtents.y) { mLargestTileGroupExtents.y = tilegroup.height(); }

	mTileGroups.push_back(TileGroupDescriptor(tilegroup_name, &tilegroup));

	delete[] tilegroup_name;
	return;
}


/**
 * 
 */
void MapFile::load(const std::string& filename)
{
	int temp = 0;

	try
	{
		auto _f = Utility<Filesystem>::get().open(filename);
		auto stream = _f.raw_bytes();
		StreamReader stream_reader(stream);

		stream_reader.read(&mMapHeadInfo, sizeof(mMapHeadInfo));

		// Update map header fields
		mMapHeadInfo.tileHeight = RoundUpPowerOf2(mMapHeadInfo.tileHeight);

		mTileWidth = 1 << mMapHeadInfo.lgTileWidth;	// Calculate map width
		mTileHeight = mMapHeadInfo.tileHeight;

		int cellCount = mTileWidth * mTileHeight;
		mTileData = new int[cellCount];

		// Read in the tile data
		stream_reader.read(mTileData, cellCount * MAP_CHUNK_SIZE);
		stream_reader.read(&mClipRect, sizeof(mClipRect));

		mTilesetManager = new TileSetManager(mMapHeadInfo.numTileSets, &stream_reader);

		readTag(&stream_reader, mMapHeadInfo.tag);
		// Something about units between reads of the tag.
		readTag(&stream_reader, mMapHeadInfo.tag);

		// Load tile groups from file
		int group_count = 0;
		stream_reader.read(&group_count, MAP_CHUNK_SIZE);
		stream_reader.read(&temp, MAP_CHUNK_SIZE);

		for (int i = 0; i < group_count; i++)
		{
			int width = 0, height = 0;
			stream_reader.read(&width, MAP_CHUNK_SIZE);
			stream_reader.read(&height, MAP_CHUNK_SIZE);

			//TileGroupInfo _tgi;
			TileGroup* _tg = new TileGroup(width, height, mTilesetManager);
			
			// Read in the tile data
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					stream_reader.read(&temp, MAP_CHUNK_SIZE);
					_tg->index(x, y, temp);
				}
			}

			_readTileGroupName(stream_reader, *_tg);
		}
	}
	catch (const std::runtime_error& e)
	{
		auto errorMsg = std::string("Error loading map: ") + e.what();
		std::cout << errorMsg << std::endl;
		throw std::runtime_error(errorMsg);
	}
	catch (...)
	{
		throw std::runtime_error("Error loading map");
	}
}


/**
 * 
 */
void MapFile::save(const std::string& filename)
{
	/*
	int numBytesWritten;

	// Check if flag to write saved game header is set
	if ((saveFlags & SavedGameHeader) == SavedGameHeader)
	{
		// Write a saved game header **TODO**
	}

	// Update number of tile sets in header structure
	tileSetManager->get_NumTileSets(&mapHeadInfo.numTileSets);
	// Write the .map file header
	stream->Write(sizeof(mapHeadInfo), (int)&mapHeadInfo, &numBytesWritten);

	// Write the tile data
	stream->Write(tileWidth*mapHeadInfo.tileHeight*4, (int)tileData, &numBytesWritten);

	// Write the clipping rect
	stream->Write(sizeof(clipRect), (int)&clipRect, &numBytesWritten);


	// Save the tile set info
	tileSetManager->Save(stream);

	// Write the tag from the header
	stream->Write(4, (int)&mapHeadInfo.tag, &numBytesWritten);

	// Check if Unit data is to be written to the stream
	if ((saveFlags & Units) == Units)
	{
		// Write Unit data to the file **TODO**
	}

	// Write the tag from the header
	stream->Write(4, (int)&mapHeadInfo.tag, &numBytesWritten);

	// Check if tile group data is to be written to the stream
	if ((saveFlags & TileGroups) == TileGroups)
	{
		// Save tile group info
		// ********************
		int i;
		int temp = 0;

		// Write the header
		stream->Write(4, (int)&numTileGroups, &numBytesWritten);
		stream->Write(4, (int)&temp, &numBytesWritten);

		// Write all the group info
		for (i = 0; i < numTileGroups; i++)
		{
			TileGroup *tg;
			int width, height;
			int x, y;

			// Get the tile group's dimensions
			tg = tileGroupInfo[i].tileGroup;
			tg->get_TileWidth(&width);
			tg->get_TileHeight(&height);

			// Write the tile group's dimensions
			stream->Write(4, (int)&width, &numBytesWritten);
			stream->Write(4, (int)&height, &numBytesWritten);
			
			// Write the tile group data
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					// Get the mapping index
					tg->get_MappingIndex(x, y, &temp);
					// Store the mapping index
					stream->Write(4, (int)&temp, &numBytesWritten);
				}
			}
			
			// Write the tile group's name
			temp = tileGroupInfo[i].nameLen;
			stream->Write(4, (int)&temp, &numBytesWritten);
			stream->Write(temp, (int)tileGroupInfo[i].name, &numBytesWritten);
		}
	}

	*status = 0;	// Success
	return S_OK;	// Success
	*/
}
