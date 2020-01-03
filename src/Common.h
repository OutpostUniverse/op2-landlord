#pragma once

#include "Stream/StreamReader.h"

#include <NAS2D/NAS2D.h>

using namespace NAS2D;

#include <string>
#include <memory>

#include <SDL.h>
// ===========================================================================
// = ENUMERATIONS
// ===========================================================================

/**
 * 
 */
enum MousePointer
{
	POINTER_NORMAL,
	POINTER_FILL,
	POINTER_ERASE
};


enum BaseTerrain
{
	TERRAIN_MUD,
	TERRAIN_ROCK,
	TERRAIN_SAND
};


// ===========================================================================
// = CONSTANTS
// ===========================================================================
const int TILE_SIZE			= 32;	/**< Tile size in pixels. */
const int MAP_CHUNK_SIZE	= 4;	/**< Size of an int value, in bytes. Since it can potentially change
										 between platforms, this is used instead of sizeof(int). */
const int NO_SELECTION		= -1;	/**< Indicates no selection. */


// ===========================================================================
// = COMMON FUNCTIONS
// ===========================================================================
void flipBool(bool& b);

void bevelBox(int x, int y, int w, int h, bool sunk = false, const Color& bgcolor = Color(180, 180, 180, 255));

std::string TrimString(const std::string& src, const std::string& c = " \r\n");

int RoundUpPowerOf2(int num);
int LogBase2(int num);

void readTag(StreamReader* in, int tag);
int gridLocation(int point, int cameraPoint, int viewportDimension);

bool pointInRect_f(int x, int y, const NAS2D::Rectangle_2df& rect);
bool pointInRect_f(int x, int y, float rectX, float rectY, float rectW, float rectH);
