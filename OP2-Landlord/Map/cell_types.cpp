#include "cell_types.h"

using namespace NAS2D;

bool	CELL_TYPE_ARRAYS_INT = false;						/**< Flag to check to ensure that the tables are initialized properly. */

std::array<std::string, CELL_TYPE_SIZE> CELL_TYPE_NAMES;	/**< Array of CellType names. */


/**
 * Gets a string from a CellType.
 */
const std::string& getCellTypeString(CellType type)
{
	#ifdef _DEBUG
	if (!CELL_TYPE_ARRAYS_INT) { throw std::runtime_error("getOverlayString(): fillCellTypes() not called."); }
	#endif

	return CELL_TYPE_NAMES[type];
}


/**
 * Fills the cell type tables used for name translation
 * and overlay coordinates.
 * 
 * \note	Must be called before getCellTypeString or getOverlayCoordinates.
 */
void fillCellTypes()
{
	CELL_TYPE_NAMES[CELL_TYPE_FAST_PASSABLE_1] = "Fast Passable 1";
	CELL_TYPE_NAMES[CELL_TYPE_IMPASSABLE_2] = "Impassable 2";
	CELL_TYPE_NAMES[CELL_TYPE_SLOW_PASSABLE_1] = "Slow Passable 1";
	CELL_TYPE_NAMES[CELL_TYPE_SLOW_PASSABLE_2] = "Slow Passable 2";
	CELL_TYPE_NAMES[CELL_TYPE_MEDIUM_PASSABLE_1] = "Medium Passable 1";
	CELL_TYPE_NAMES[CELL_TYPE_MEDIUM_PASSABLE_2] = "Medium Passable 2";
	CELL_TYPE_NAMES[CELL_TYPE_IMPASSABLE_1] = "Impassable 1";
	CELL_TYPE_NAMES[CELL_TYPE_FAST_PASSABLE_2] = "Fast Passable 2";
	CELL_TYPE_NAMES[CELL_TYPE_NORTH_CLIFF] = "North Cliff";
	CELL_TYPE_NAMES[CELL_TYPE_CLIFF_HIGH_SIDE] = "Cliff High Side";
	CELL_TYPE_NAMES[CELL_TYPE_CLIFF_LOW_SIDE] = "Cliff Low Side";
	CELL_TYPE_NAMES[CELL_TYPE_VENT_AND_FUMAROLE] = "Vent / Fumarole";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_12] = "ZPad 12";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_13] = "ZPad 13";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_14] = "ZPad 14";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_15] = "ZPad 15";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_16] = "ZPad 16";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_17] = "ZPad 17";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_18] = "ZPad 18";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_19] = "ZPad 19";
	CELL_TYPE_NAMES[CELL_TYPE_ZPAD_20] = "ZPad 20";
	CELL_TYPE_NAMES[CELL_TYPE_DOZED] = "Dozed";
	CELL_TYPE_NAMES[CELL_TYPE_RUBBLE] = "Rubble";
	CELL_TYPE_NAMES[CELL_TYPE_NORMAL_WALL] = "Normal Wall";
	CELL_TYPE_NAMES[CELL_TYPE_MICROBE_WALL] = "Microbe Wall";
	CELL_TYPE_NAMES[CELL_TYPE_LAVA_WALL] = "Lava Wall";
	CELL_TYPE_NAMES[CELL_TYPE_TUBE_0] = "Tube 0";
	CELL_TYPE_NAMES[CELL_TYPE_TUBE_1] = "Tube 1";
	CELL_TYPE_NAMES[CELL_TYPE_TUBE_2] = "Tube 2";
	CELL_TYPE_NAMES[CELL_TYPE_TUBE_3] = "Tube 3";
	CELL_TYPE_NAMES[CELL_TYPE_TUBE_4] = "Tube 4";
	CELL_TYPE_NAMES[CELL_TYPE_TUBE_5] = "Tube 5";

	CELL_TYPE_ARRAYS_INT = true;
}
