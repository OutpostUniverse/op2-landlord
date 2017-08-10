#include "TileSetManager.h"
#include "../Common.h"

const char TILESET_TAG[] = "TILE SET\x01A";

/**
 * C'Tor
 */
TileSetManager::TileSetManager(const std::string& tilesetName)
{
	// Allocate memory for the tile sets and clear it out
	tileSetInfo = new TileSetInfo[numTileSets];
	memset(tileSetInfo, 0, sizeof(*tileSetInfo) * numTileSets);
}


/**
 * C'Tor
 */
TileSetManager::TileSetManager(int numTileSets, StreamReader *inStream) : numTileSets(numTileSets)
{
	if (Load(inStream) != 0)
	{
		FreeMemory();
	}
}


/**
 * D'Tor
 */
TileSetManager::~TileSetManager()
{
	FreeMemory();
}


/**
 * 
 */
int TileSetManager::tileSets()
{
	return numTileSets;
}


/**
 * 
 */
void TileSetManager::validateIndex(int index)
{
	if ((index < 0) || (index >= numTileSets))
	{
		throw std::runtime_error("TileSetManager::tileSetName(): Invalid index");
	}
}


/**
 * 
 */
const std::string& TileSetManager::tileSetName(int index)
{
	validateIndex(index);
	return tileSetInfo[index].wideTileSetName;
}


/**
 * 
 */
TileSet* TileSetManager::tileSet(int index)
{
	validateIndex(index);
	return tileSetInfo[index].tileSet;
}


/**
 * 
 */
int TileSetManager::addTileSet(const std::string& tileSetName)
{
	/*
	TileSetInfo *tileInfo;
	TileSet *tileSet;
	unsigned int lenString;
	int tileSetIndex;

	// Cache the tileSetName string length
	lenString = SysStringLen(tileSetName);
	*index = -1;

	// Check if tile set is already loaded
	for (tileSetIndex = 0; tileSetIndex < numTileSets; tileSetIndex++)
	{
		tileInfo = &tileSetInfo[tileSetIndex];		// Cache the tileSetInfo address

		// Check if input string matches this entry
		if ( (lenString == SysStringLen(tileInfo->wideTileSetName))
			&& (memcmp(tileSetName, tileInfo->wideTileSetName, lenString*2) == 0) )
		{
			// Tile set already loaded
			*index = tileSetIndex;

			PostErrorMsg(L"Tile set already loaded.");
			return E_FAIL;
		}
	}

	// Try to load the tile set
	tileSource->LoadTileSet(tileSetName, &tileSet);
	if (tileSet == NULL)
	{
		// Failed to load the tile set
		PostErrorMsg(L"Failed to load the tile set.");
		return E_FAIL;
	}

	// Find an empty tile set slot
	for (tileSetIndex = 0; tileSetIndex < numTileSets; tileSetIndex++)
	{
		if (tileSetInfo[tileSetIndex].stringLength == 0)
		{
			// Empty slot found. Add tile set here
			break;	// Fill in entries after the loop
		}
	}

	if (tileSetIndex >= numTileSets)
	{
		// No empty tile set slot found. Try to extend the tile set list.
		TileSetInfo *newInfo = new TileSetInfo[numTileSets+1];
		if (newInfo == NULL)
		{
			// Failed to allocate new space for tile mappings
			// Release the loaded tile set
			tileSource->UnloadTileSet(tileSetName);
			tileSet->Release();

			PostErrorMsg(L"Could not allocate space for new tile set array.");

			return E_FAIL;
		}
		// Copy the tile set info to the new array
		memcpy(newInfo, tileSetInfo, sizeof(*tileSetInfo)*numTileSets);
		// Release the old data
		delete [] tileSetInfo;
		// Update the pointer to the new array
		tileSetInfo = newInfo;
		// Increment number of tile sets in array
		numTileSets++;
	}

	// Fill in tile set info entry
	*index = tileSetIndex;
	tileInfo = &tileSetInfo[tileSetIndex];		// Cache pointer to entry
	// Store string length
	tileInfo->stringLength = lenString;
	// Allocate a new BSTR to cache the name
	tileInfo->wideTileSetName = SysAllocString(tileSetName);
	// Allocate a new char array to hold the tile set name
	tileInfo->tileSetName = new char[lenString+1];
	tileInfo->tileSetName[lenString] = 0;	// NULL terminate it
	// Convert the BSTR to ASCII
	WideCharToMultiByte(CP_ACP, 0, tileSetName, lenString, tileInfo->tileSetName, lenString, 0, 0);
	// Initialize tile set variables
	tileSet->get_NumTiles(&tileInfo->numTiles);
	tileInfo->tileSet = tileSet;

	// Return success
	return S_OK;
	*/
	return 0;
}


/**
 * 
 */
int TileSetManager::removeTileSet(const std::string& tileSetName)
{
	/*
	TileSetInfo *tileInfo;
	unsigned int lenString;
	int i;

	lenString = SysStringLen(tileSetName);	// Cache the tileSetName string length
	*index = -1;
	// Search for the input string in the array
	for (i = 0; i < numTileSets; i++)
	{
		tileInfo = &tileSetInfo[i];		// Cache the tileSetInfo address

		// Check if input string matches this entry
		if ( (lenString == SysStringLen(tileInfo->wideTileSetName))
			&& (memcmp(tileSetName, tileInfo->wideTileSetName, lenString*2) == 0) )
		{
			// Strings match. Delete this entry
			*index = i;
			// Release the tile set
			tileSource->UnloadTileSet(tileSetName);
			// Release the cached tile set name BSTR
			SysFreeString(tileInfo->wideTileSetName);
			tileInfo->wideTileSetName = NULL;
			// Free the ascii string
			delete [] tileInfo->tileSetName;
			tileInfo->tileSetName = NULL;
			// Reset all other variables
			tileInfo->stringLength = 0;
			tileInfo->numTiles = 0;
			tileInfo->tileSet->Release();
			tileInfo->tileSet = NULL;
			// Break out of the loop
			break;
		}
	}

	// **TODO** Clear out the mapping array elements which referred to this tile set

	return S_OK;
	*/
	return 0;
}


/**
 * 
 */
int TileSetManager::MapInTiles(int tileSetIndex, int startTile, int numTiles /* [retval][out] int* mappingIndex*/)
{
	/*
	// Initialize returned mapping index to invalid
	*mappingIndex = -1;

	// Error check
	// ***********
	// Make sure the specified tile set is valid
	if (tileSetIndex >= numTileSets)
	{
		PostErrorMsg(L"Invalid tile set specified.");
		return E_INVALIDARG;
	}
	// Check for an invalid tile range
	if ((startTile < 0) || (numTiles < 1))
	{
		PostErrorMsg(L"Invalid range. Value of startTile must be nonnegative (>=0) and value of numTiles must be positive (>0).");
		return E_INVALIDARG;
	}
	// Make sure the range of tiles is contained in the specified tile set (Note: comparison must be > here)
	if ((startTile + numTiles) > tileSetInfo[tileSetIndex].numTiles)
	{
		PostErrorMsg(L"Invaild range. The tile set does not contain those tiles.");
		return E_INVALIDARG;
	}
	// Check if there is room to add the new tile mapping entries
	if ((numMappings + numTiles) >= 2048)
	{
		PostErrorMsg(L"Only a maximum of 2048 tiles can be used.");
		return E_FAIL;
	}

	// Add in the new tile set mappings
	// ********************************
	// Allocate space for new mapping array
	TileSetTileMapping *newMapping = new TileSetTileMapping[numMappings + numTiles];
	// Check for allocation error
	if (newMapping == NULL)
	{
		PostErrorMsg(L"Could not allocate memory for new mapping array.");
		return E_FAIL;
	}

	// Copy the old array into the new one
	memcpy(newMapping, mapping, sizeof(mapping[0])*numMappings);
	// Release the old memory
	delete [] mapping;
	// Update the array pointer
	mapping = newMapping;

	// Initialize new entries
	int i;
	*mappingIndex = numMappings;
	for (i = 0; i < numTiles; i++)
	{
		mapping[numMappings+i].tileSetIndex = tileSetIndex;
		mapping[numMappings+i].tileIndex = startTile + i;
		mapping[numMappings+i].numTileReplacements = 0;
		mapping[numMappings+i].cycleDelay = 0;
	}
	// Update the number of mappings
	numMappings += numTiles;

	// Return success
	return S_OK;
	*/

	return 0;
}


/**
 * \note	Use -1 for don't care conditions on numTileReplacements and cycleDelay.
 *			Use -2 for a nonzero condition on numTileReplacements and cycleDelay.
 */
int TileSetManager::getMappingIndex(int tileSetIndex, int tileIndex, int numTileReplacements, int cycleDelay)
{
	int i;

	for (i = 0; i < numMappings; i++)
	{
		TileSetTileMapping &map = mapping[i];
		if ((map.tileSetIndex == tileSetIndex) && (map.tileIndex == tileIndex))
		{
			// TileSet and TileSetIndex match.
			// Check remaining parameters
			if ((numTileReplacements == -1) || (numTileReplacements == map.numTileReplacements) || ((numTileReplacements == -2) && (map.numTileReplacements != 0)))
			{
				if ((cycleDelay == -1) || (cycleDelay == map.cycleDelay) || ((cycleDelay == -2) && (map.cycleDelay != 0)))
				{
					// Match found
					return i;
				}
			}
		}
	}

	return -1;
}


/**
 * 
 */
int TileSetManager::get_NumMappings()
{
	return numMappings;
}


/**
 * 
 */
void TileSetManager::validateMappingIndex(int index)
{
	if ((index < 0) || (index >= numMappings))
	{
		throw std::runtime_error("TileSetManager::validateMappingIndex(): Invalid mapping index.");
	}
}


/**
 * 
 */
int TileSetManager::get_TileSetIndex(int mappingIndex)
{
	validateMappingIndex(mappingIndex);
	return mapping[mappingIndex].tileSetIndex;
}


/**
 * 
 */
void TileSetManager::put_TileSetIndex(int mappingIndex, int tileSetIndex)
{
	validateMappingIndex(mappingIndex);
	mapping[mappingIndex].tileSetIndex = tileSetIndex;
}


/**
 * 
 */
int TileSetManager::get_TileSetTileIndex(int mappingIndex)
{
	validateMappingIndex(mappingIndex);
	return mapping[mappingIndex].tileIndex;
}


/**
 * 
 */
void TileSetManager::put_TileSetTileIndex(int mappingIndex, int tileSetTileIndex)
{
	validateMappingIndex(mappingIndex);
	mapping[mappingIndex].tileIndex = tileSetTileIndex;
}


/**
 * 
 */
int TileSetManager::get_NumTileReplacements(int mappingIndex)
{
	validateMappingIndex(mappingIndex);
	return mapping[mappingIndex].numTileReplacements;
}


/**
 * 
 */
void TileSetManager::put_NumTileReplacements(int mappingIndex, int numTileReplacements)
{
	validateMappingIndex(mappingIndex);
	mapping[mappingIndex].numTileReplacements = numTileReplacements;
}


/**
 * 
 */
int TileSetManager::get_CycleDelay(int mappingIndex)
{
	validateMappingIndex(mappingIndex);
	return mapping[mappingIndex].cycleDelay;
}


/**
 * 
 */
void TileSetManager::put_CycleDelay(int mappingIndex, int cycleDelay)
{
	validateMappingIndex(mappingIndex);
	mapping[mappingIndex].cycleDelay = cycleDelay;
}


/**
 * 
 */
int TileSetManager::get_NumTerrains()
{
	return mTerrainCount;
}


/**
 * 
 */
void TileSetManager::set_NumTerrains(int numTerrains)
{
	if (numTerrains < 1)
	{
		throw std::runtime_error("TileSetManager::set_NumTerrains(): invalid value.");
	}

	// Allocate space for new terrain array
	TerrainType *newTerrain = new TerrainType[numTerrains];

	// Zero it
	memset(newTerrain, 0, sizeof(terrain[0])*numTerrains);

	// Copy the old array into the new one
	if (numTerrains < mTerrainCount)
	{
		memcpy(newTerrain, terrain, sizeof(terrain[0]) * numTerrains);
	}
	else
	{
		memcpy(newTerrain, terrain, sizeof(terrain[0]) * mTerrainCount);
	}

	// Release the old memory
	delete[] terrain;
	// Update the array pointer & numTerrains var
	terrain = newTerrain;
	mTerrainCount = numTerrains;
}


/**
 * 
 */
void TileSetManager::validateTerrainIndex(int index)
{
	if (index < 0 || index > mTerrainCount)
	{
		throw std::runtime_error("TileSetManager: Invalid terrain index.");
	}
}


/**
 * 
 */
int TileSetManager::get_TerrainStartTile(int terrainIndex)
{
	validateTerrainIndex(terrainIndex);

	return terrain[terrainIndex].firstTile;
}


/**
 * 
 */
void TileSetManager::put_TerrainStartTile(int terrainIndex, int startMapping)
{
	validateTerrainIndex(terrainIndex);
	terrain[terrainIndex].firstTile = startMapping;
}


/**
 * 
 */
int TileSetManager::get_TerrainEndTile(int terrainIndex)
{
	validateTerrainIndex(terrainIndex);
	return terrain[terrainIndex].lastTile;
}


/**
 * 
 */
void TileSetManager::put_TerrainEndTile(int terrainIndex, int endMapping)
{
	validateTerrainIndex(terrainIndex);
	terrain[terrainIndex].lastTile = endMapping;
}


/**
 * 
 */
void TileSetManager::FreeMemory()
{
	if (mapping) { delete[] mapping; }
	if (terrain) { delete[] terrain; }

	if (tileSetInfo)
	{
		// Free all the string data
		for (int i = 0; i < numTileSets; i++)
		{
			TileSetInfo* tileSet = &tileSetInfo[i];

			if (tileSet->tileSetName)
			{
				// Free the memory for the string
				delete[] tileSetInfo[i].tileSetName;
			}
		}

		delete[] tileSetInfo;
	}
}


/**
 * 
 */
int TileSetManager::Save(StreamWriter* stream)
{
	/*
	int i;
	int numBytesWritten;

	// Write the tile set file list
	for (i = 0; i < numTileSets; i++)
	{
		// Write the length of the string
		stream->Write(4, (int)&tileSetInfo[i].stringLength, &numBytesWritten);
		if (tileSetInfo[i].stringLength)
		{
			// String exists. Write it.
			stream->Write(tileSetInfo[i].stringLength, (int)tileSetInfo[i].tileSetName, &numBytesWritten);
			// Write the number of tiles in this tile set
			stream->Write(4, (int)&tileSetInfo[i].numTiles, &numBytesWritten);
		}
	}

	// Write the Tile Set info
	// Write the section tag "TILE SET", 0x1A, 0
	stream->Write(0xA, (int)tagTileSet, &numBytesWritten);
	// Write the number of tile set mappings
	stream->Write(4, (int)&numMappings, &numBytesWritten);
	// Write the tile set tile mappings
	stream->Write(sizeof(*mapping)*numMappings, (int)mapping, &numBytesWritten);
	// Write the number of terrain types
	stream->Write(4, (int)&numTerrains, &numBytesWritten);
	// Write the terrain type info
	stream->Write(sizeof(*terrain)*numTerrains, (int)terrain, &numBytesWritten);
*/
	return 0;
}


/**
 * 
 */
int TileSetManager::Load(StreamReader* stream)
{
	char scratch[64] = { '\0' }; // Used to read the "TILE SET" tag

	try
	{
		// Allocate space for the tile set info
		tileSetInfo = new TileSetInfo[numTileSets];
		// Load Tile Set file list
		for (int i = 0; i < numTileSets; i++)
		{
			// Cache the tileSet address
			TileSetInfo* tileSet = &tileSetInfo[i];

			// Read the length of the string
			int stringLen = 0;
			stream->read(&stringLen, 4);

			// Initialize struct variables
			tileSet->stringLength = stringLen;		// Store the string length
			tileSet->tileSetName = NULL;		// NULL string pointer
			tileSet->numTiles = 0;				// No loaded tiles in this tile set
			tileSet->tileSet = NULL;			// NULL ITileSet pointer

			if (stringLen > 0)
			{
				tileSet->tileSetName = new char[stringLen + 1];
				tileSet->tileSetName[stringLen] = 0;

				stream->read(tileSet->tileSetName, stringLen);

				tileSetInfo[i].wideTileSetName = tileSet->tileSetName;

				tileSet->tileSet = new TileSet(tileSet->tileSetName);

				// Read the number of tiles stored in this tile set
				stream->read(&tileSet->numTiles, 4);
			}
		}


		// Load Tile Set mapping info
		// Read the string "TILE SET", 0x1A, 0
		stream->read(scratch, 0xA);
		if (strncmp(scratch, TILESET_TAG, 0xA))
		{
			throw std::runtime_error("TileSetManager::Load(): Could not find \"TILE SET\" tag.");
		}

		// Read the number of tile set tile mappings
		stream->read(&numMappings, 4);
		mapping = new TileSetTileMapping[numMappings];

		// Read the tile set tile mapping info
		stream->read(mapping, sizeof(mapping[0]) * numMappings);

		// Read the number of terrain types
		stream->read(&mTerrainCount, 4);

		// Allocate space for terrain type info
		terrain = new TerrainType[mTerrainCount];

		// Read the terrain type info
		stream->read(terrain, sizeof(*terrain) * mTerrainCount);
	}
	catch (...)
	{
		return 1;	// Failed to load file
	}

	return 0;
}
