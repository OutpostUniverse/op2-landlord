#include "MapFile.h"

#include "../Common.h"

#include "../Stream/StreamReader.h"

#include <iostream>

#include <NAS2D/NAS2D.h>

using namespace NAS2D;


/**
 * C'tor
 * 
 * Loads an existing map file.
 */
MapFile::MapFile(const std::string& mapName)
{
	// Initialize variables
	if (LoadMap(mapName) != 0)
	{
		throw std::runtime_error("Error loading map data from stream.");
	}
}


/**
 * C'tor
 * 
 * Creates a new map from scratch.
 */
MapFile::MapFile(const std::string& tsetName, int width, int height)
{
	mTileWidth = RoundUpPowerOf2(width);
	mTileHeight = RoundUpPowerOf2(height);

	// Check size is valid
	if (mTileWidth < 16 || mTileWidth > 512) { throw std::runtime_error("MapFile(): Invalid map size."); }
	if (mTileHeight < 16 || mTileHeight > 256) { throw std::runtime_error("MapFile(): Invalid map size."); }

	initMapHeader();

	// Initialize the clipping rectangle
	if (mMapHeadInfo.lgTileWidth >= 9)
	{
		mClipRect.x1 = -1;
		mClipRect.y1 = 0;
		mClipRect.x2 = 0x7FFFFFFF;
		mClipRect.y2 = mTileHeight - 2;
	}
	else
	{
		mClipRect.x1 = 32;
		mClipRect.y1 = 0;
		mClipRect.x2 = mTileWidth + 0x1F;
		mClipRect.y2 = mTileHeight - 2;
	}

	// Allocate space for data
	mTileData = new int[mTileWidth * mTileHeight];

	// Clear tile data to 0
	memset(mTileData, 0, mTileWidth * mTileHeight);
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

	if (mTileGroupInfo)
	{
		for (int i = 0; i < mTileGroupCount; i++)
		{
			delete mTileGroupInfo[i].tileGroup;
			delete mTileGroupInfo[i].name;
		}

		delete[] mTileGroupInfo;
	}
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


int MapFile::width()
{
	return mTileWidth;
}


int MapFile::height()
{
	return mTileHeight;
}

void MapFile::validateCoords(int x, int y) const
{
	// Error check array indicies
	if ((x < 0) || (x >= mTileWidth) || (y < 0) || (y >= mTileHeight))
	{
		throw std::runtime_error("MapFile::Index(): Invalid tile location.");
	}
}


int MapFile::index(int x, int y)
{
	validateCoords(x, y);

	int tileXUpper = x >> 5;
	int tileXLower = y & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	return (mTileData[tileOffset] & 0x0000FFE0) >> 5;;
}


const Color_4ub& MapFile::tile_color(int x, int y)
{
	validateCoords(x, y);

	TileSetManager::TileSetTileMapping* tileMap = &mTilesetManager->mMapping[tset_index(x, y)];
	TileSet* tileSet = mTilesetManager->mTileSetInfo[tileMap->tileSetIndex].tileSet;

	return tileSet->color(tileMap->tileIndex);
}


void MapFile::index(int x, int y, int index)
{
	validateCoords(x, y);

	// Check for invalid mapping index
	if ((index & ~0x7FF) != 0)	// Only 11 bits allowed
	{
		throw std::runtime_error("MapFile::Index(): Invalid tile index.");
	}

	// **TODO** Check if index is out of array bounds?

	int tileXUpper = x >> 5;
	int tileXLower = x & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	// Set the tile mapping index
	mTileData[tileOffset] = (mTileData[tileOffset] & 0xFFFF001F) | (index << 5);
}


int MapFile::tset_index(int x, int y)
{
	int tileXUpper = x / 32;
	int tileXLower = x % 32;
	int tileOffset = (((tileXUpper * mTileHeight) + y) * 32) + tileXLower;

	return clamp((mTileData[tileOffset] & 0x0000FFE0) / 32, 0, mTilesetManager->mNumMappings);
}


void MapFile::updateCameraAnchorArea(int width, int height)
{
	mCameraAnchorArea(0, 0, mTileWidth * 32 - width, mTileHeight * 32 - height);
	setCamera(mCameraPosition.x(), mCameraPosition.y());
}


void MapFile::moveCamera(int x, int y)
{
	setCamera(mCameraPosition.x() + x, mCameraPosition.y() + y);
}


void MapFile::setCamera(int x, int y)
{
	mCameraPosition(clamp(x, 0, mCameraAnchorArea.width()),
					clamp(y, 0, mCameraAnchorArea.height()));
}


void MapFile::draw(int x, int y, int width, int height)
{
	int offsetX = mCameraPosition.x() / 32;
	int offsetY = mCameraPosition.y() / 32;

	int drawOffsetX = mCameraPosition.x() % 32;
	int drawOffsetY = mCameraPosition.y() % 32;

	int columns = (width / 32) + (drawOffsetX % 32);
	int rows = (height / 32) + (drawOffsetY % 32);

	int tileIndex = 0;

	TileSet* tileSet = nullptr;
	TileSetManager::TileSetTileMapping* tileMap = nullptr;
	for (int row = 0; row <= rows && row + offsetY < mTileHeight; ++row)
	{
		for (int col = 0; col <= columns && col + offsetX < mTileWidth; ++col)
		{
			tileIndex = tset_index(col + offsetX, row + offsetY);

			tileMap = &mTilesetManager->mMapping[tileIndex];
			tileSet = mTilesetManager->mTileSetInfo[tileMap->tileSetIndex].tileSet;
			if (tileSet)
			{
				tileSet->draw(tileMap->tileIndex, x + (col * 32) - drawOffsetX, y + (row * 32) - drawOffsetY);
			}
		}
	}
}


bool MapFile::aroundTheWorld() const
{
	return (mMapHeadInfo.lgTileWidth >= 9);
}


int MapFile::cellType(int x, int y) const
{
	validateCoords(x, y);

	int tileXUpper = x >> 5;
	int tileXLower = y & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	return mTileData[tileOffset] & 0x1F;
}


void MapFile::cellType(int x, int y, int cell_type)
{
	validateCoords(x, y);

	// Check for invalid cell type
	if ((cell_type & ~0x1F) != 0)	// Only 5 bits allowed
	{
		throw std::runtime_error("MapFile::Index(): Invalid cell type.");
	}

	int tileXUpper = x >> 5;
	int tileXLower = y & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	mTileData[tileOffset] = (mTileData[tileOffset] & 0xFFFFFFE0) | (cell_type & 0x1F);
}



bool MapFile::lavaPossible(int x, int y)
{
	validateCoords(x, y);

	int tileXUpper = x >> 5;
	int tileXLower = x & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	return (mTileData[tileOffset] >> 28) & 0x1;;
}


void MapFile::lavaPossible(int x, int y, int lavaPossible)
{
	validateCoords(x, y);

	int tileXUpper = x >> 5;
	int tileXLower = x & 0x1F;
	int tileOffset = (((tileXUpper * mTileHeight) + y) << 5) + tileXLower;

	// Set the LavaPossible bit
	mTileData[tileOffset] |= (lavaPossible & 0x1) << 28;
}


int MapFile::tileGroups() const
{
	return mTileGroupCount;
}


int MapFile::LoadMap(const std::string& mapName)
{
	// **NOTE**: Changed the 'format errors' to just post an error rather than throw an exception.
	// Although they can cause problems, it shouldn't prevent the map from loading.
	int status = 0;
	int temp = 0;

	int stream_position = 0;

	try
	{
		File _f = Utility<Filesystem>::get().open(mapName);
		File::RawByteStream stream = _f.raw_bytes();
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

		/// Clip rect (really? this can't be done based on the rest of the information?)
		stream_reader.read(&mClipRect, sizeof(mClipRect));

		mTilesetManager = new TileSetManager(mMapHeadInfo.numTileSets, &stream_reader);

		readTag(&stream_reader, mMapHeadInfo.tag);
		// Something about units between reads of the tag.
		readTag(&stream_reader, mMapHeadInfo.tag);

		// Load tile groups from file
		stream_reader.read(&mTileGroupCount, MAP_CHUNK_SIZE);
		stream_reader.read(&temp, MAP_CHUNK_SIZE);

		// Allocate space for the tile group info
		mTileGroupInfo = new TileGroupInfo[mTileGroupCount];
		// Initialize the array
		memset(mTileGroupInfo, 0, sizeof(mTileGroupInfo) * mTileGroupCount);

		// Read all group info
		for (int i = 0; i < mTileGroupCount; i++)
		{
			int width = 0, height = 0;
			int x = 0, y = 0;

			// Read the dimensions
			stream_reader.read(&width, MAP_CHUNK_SIZE);
			stream_reader.read(&height, MAP_CHUNK_SIZE);
			// Create a new TileGroup
			mTileGroupInfo[i].tileGroup = new TileGroup(width, height, mTilesetManager);

			// Read in the tile data
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < width; x++)
				{
					stream_reader.read(&temp, MAP_CHUNK_SIZE);
					mTileGroupInfo[i].tileGroup->mappingIndex(x, y, temp);
				}
			}

			// Read in the tile group name length
			stream_reader.read(&mTileGroupInfo[i].nameLen, MAP_CHUNK_SIZE);
			mTileGroupInfo[i].name = new char[mTileGroupInfo[i].nameLen + 1];

			// Read in the tile group name
			stream_reader.read(mTileGroupInfo[i].name, mTileGroupInfo[i].nameLen);
			mTileGroupInfo[i].name[mTileGroupInfo[i].nameLen] = 0;
		}
	}
	catch (const std::string& errorMsg)
	{
		std::cout << errorMsg << std::endl;
		return -1;	// Failed to load file
	}
	catch (...)
	{
		return -1;	// Failed to load file
	}

	return 0;	// Success
}


int MapFile::SaveMap(const std::string& filename, enum MapLoadSaveFormat saveFlags)
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
	return 0;
}
