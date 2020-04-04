#pragma once

/**
 * This is a list of TileGroups that are ignored when parsing an Outpost 2
 * map file. These offer no real benefit to the editor 
 */


#include <array>


std::array<std::string, 29> TILEGROUP_IGNORE_LIST = 
{
	"BLUE",
	"muddoz",
	"mudlavw",
	"mudmicw",
	"mudtubes",
	"mudwall1",
	"mudwall2",
	"mudwall3",
	"rocdoz",
	"roclavw",
	"rocmicw",
	"roctubes",
	"rocwall1",
	"rocwall2",
	"rocwall3",
	"roczlav",
	"sandoz",
	"sanlavw",
	"sanmicw",
	"santube",
	"sanwall1",
	"sanwall2",
	"sanwall3",
	"mudtube2",
	"roctube2",
	"santube2",
	"LEFT_LAVA",
	"midd_lava",
	"RIGHT_LAVA"
};
