#pragma once

#include <array>
#include <string>

#include <NAS2D/NAS2D.h>

enum CellType
{
	CELL_TYPE_FAST_PASSABLE_1	= 0x00,
	CELL_TYPE_IMPASSABLE_2		= 0x01,
	CELL_TYPE_SLOW_PASSABLE_1	= 0x02,
	CELL_TYPE_SLOW_PASSABLE_2	= 0x03,
	CELL_TYPE_MEDIUM_PASSABLE_1	= 0x04,
	CELL_TYPE_MEDIUM_PASSABLE_2	= 0x05,
	CELL_TYPE_IMPASSABLE_1		= 0x06,
	CELL_TYPE_FAST_PASSABLE_2	= 0x07,
	CELL_TYPE_NORTH_CLIFF		= 0x08,
	CELL_TYPE_CLIFF_HIGH_SIDE	= 0x09,
	CELL_TYPE_CLIFF_LOW_SIDE	= 0x0A,
	CELL_TYPE_VENT_AND_FUMAROLE	= 0x0B,
	CELL_TYPE_ZPAD_12			= 0x0C,
	CELL_TYPE_ZPAD_13			= 0x0D,
	CELL_TYPE_ZPAD_14			= 0x0E,
	CELL_TYPE_ZPAD_15			= 0x0F,
	CELL_TYPE_ZPAD_16			= 0x10,
	CELL_TYPE_ZPAD_17			= 0x11,
	CELL_TYPE_ZPAD_18			= 0x12,
	CELL_TYPE_ZPAD_19			= 0x13,
	CELL_TYPE_ZPAD_20			= 0x14,
	CELL_TYPE_DOZED				= 0x15,
	CELL_TYPE_RUBBLE			= 0x16,
	CELL_TYPE_NORMAL_WALL		= 0x17,
	CELL_TYPE_MICROBE_WALL		= 0x18,
	CELL_TYPE_LAVA_WALL			= 0x19,
	CELL_TYPE_TUBE_0			= 0x01A,
	CELL_TYPE_TUBE_1			= 0x01B,
	CELL_TYPE_TUBE_2			= 0x01C,
	CELL_TYPE_TUBE_3			= 0x01D,
	CELL_TYPE_TUBE_4			= 0x01E,
	CELL_TYPE_TUBE_5			= 0x01F,

	CELL_TYPE_SIZE
};


const std::string& getCellTypeString(CellType type);

void fillCellTypes();
