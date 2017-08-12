#pragma once

#include "NAS2D/NAS2D.h"

using namespace NAS2D;

#include <string>
#include <memory>

#include <SDL.h>

const int TILE_SIZE			= 32;	/**< Tile size in pixels. */
const int MAP_CHUNK_SIZE	= 4;	/**< Size of an int value, in bytes. Since it can potentially change
										 between platforms, this is used instead of sizeof(int). */


enum MousePointer
{
	POINTER_NORMAL,
	POINTER_FILL,
	POINTER_ERASE
};

void flipBool(bool& b);

void bevelBox(int x, int y, int w, int h, bool sunk = false, const Color_4ub& bgcolor = Color_4ub(180, 180, 180, 255));

std::string TrimString(const std::string& src, const std::string& c = " \r\n");

void DrawPixel(SDL_Surface* srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A);
void BlendPixel(SDL_Surface* srf, int x, int y, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

int RoundUpPowerOf2(int num);
int LogBase2(int num);

class StreamReader;

/**
 * Helper function.
 * 
 * Reads a map tag.
 * 
 * \note throws if tag is incorrect.
 */
void readTag(StreamReader* in, int tag);