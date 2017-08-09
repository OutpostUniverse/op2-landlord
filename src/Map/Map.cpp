#include "Map.h"

#include "../Common.h"

#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;
using namespace NAS2D;

const Rectangle_2d	CELL_DIMENSIONS		= Rectangle_2d(0, 0, 32, 32);

const std::string	MAP_DRIVER_VERSION	= "0.30";

const int			EDGE_MARGIN			= 10;


/**
 * C'tor
 */
Map::Map(const string& mapPath):	mField(0, 0),
									mViewport(0, 0, static_cast<int>(Utility<Renderer>::get().width()), static_cast<int>(Utility<Renderer>::get().height())),
									mCameraFocus(nullptr),
									mDrawBg(true),
									mDrawBgDetail(true),
									mDrawDetail(true),
									mDrawForeground(true),
									mDrawCollision(false),
									mShowLinks(false),
									mShowTitlePlaque(false),
									mEdgeExit(false)
{
	load(mapPath);
}


/**
 * C'tor
 */
Map::Map(const string& name, const string& tsetPath, int width, int height):	mName(name),
																				mField(width, height),
																				mTileset(tsetPath, CELL_DIMENSIONS.width(), CELL_DIMENSIONS.height()),
																				mViewport(0, 0, static_cast<int>(Utility<Renderer>::get().width()), static_cast<int>(Utility<Renderer>::get().height())),
																				mCameraSpace(0, 0, mField.width() * mTileset.width() - mViewport.width(), mField.height() * mTileset.height() - mViewport.height()),
																				mCameraFocus(nullptr),
																				mDrawBg(true),
																				mDrawBgDetail(true),
																				mDrawDetail(true),
																				mDrawForeground(true),
																				mDrawCollision(false),
																				mShowLinks(false),
																				mShowTitlePlaque(false),
																				mEdgeExit(false)
{}


/**
 * D'tor
 */
Map::~Map()
{}


/**
 * Gets the name of the map.
 */
const string& Map::name() const
{
	return mName;
}


/**
 * Sets the name of the map.
 */
void Map::name(const string& name)
{
	mName = name;
}


/**
 * Gets a path to a background music track.
 */
const string& Map::bgMusic() const
{
	return mBgMusic;
}


/**
 * Sets a path to a background music track.
 */
void Map::bgMusic(const string& path)
{
	mBgMusic = path;
}


/**
 * Gets a bool value indicating that a title plaque should be displayed.
 */
bool Map::showTitlePlaque() const
{
	return mShowTitlePlaque;
}


/**
 * Sets a bool value indicating that a title plaque should be displayed.
 */
void Map::showTitlePlaque(bool _b)
{
	mShowTitlePlaque = true;
}


/**
 * Internal function used by Map.
 * 
 * Determines a grid location based on a point, camera offset, tile size and viewport size.
 */
int Map::gridLocation(int point, int cameraPoint, int tileDimension, int viewportDimension) const
{
	return ((point - -(cameraPoint % tileDimension)) / tileDimension) % viewportDimension;
}


void Map::drawCollision(bool draw)
{
	mDrawCollision = draw;
}


void Map::showLinks(bool show)
{
	mShowLinks = show;
}


void Map::drawBg(bool draw)
{
	mDrawBg = draw;
}


void Map::drawBgDetail(bool draw)
{
	mDrawBgDetail = draw;
}


void Map::drawDetail(bool draw)
{
	mDrawDetail = draw;
}


void Map::drawForeground(bool draw)
{
	mDrawForeground = draw;
}


void Map::cameraFocus(Entity* entity)
{
	mCameraFocus = entity;
}


/**
 * Add an Entity to the ent list.
 */
void Map::push_entity(Entity* entity)
{
	for(size_t i = 0; i < mEntityList.size(); i++)
	{
		if(mEntityList[i] == entity)
			return;
	}

	mEntityList.push_back(entity);
}


/**
 * Removes an Entity from the ent list.
 */
void Map::pop_entity(Entity* entity)
{
	for(size_t i = 0; i < mEntityList.size(); i++)
	{
		if(mEntityList[i] == entity)
		{
			mEntityList.erase(mEntityList.begin() + i);
			return;
		}
	}
}


/**
 * Sets the viewport area.
 */
void Map::viewport(const Rectangle_2d& _r)
{
	mViewport = _r;
	mCameraSpace(0, 0, (width() * mTileset.width()) - mViewport.width(), (height() * mTileset.height()) - mViewport.height());
}


/**
 * Gets the width of the Map in Cell's.
 */
int Map::width() const
{
	return mField.width();
}


/**
 * Gets the height of the Map in Cell's.
 */
int Map::height() const
{
	return mField.height();
}


/**
 * Gets the map's world width.
 */
int Map::world_width() const
{
	return mField.width() * CELL_DIMENSIONS.width();
}


/**
 * Gets the map's world height.
 */
int Map::world_height() const
{
	return mField.height() * CELL_DIMENSIONS.height();
}


/**
 * Gets a rectangle defining the map's world area.
 */
Rectangle_2df Map::world_size() const
{
#if defined(_DEBUG)
	Rectangle_2df rect(0.0f, 0.0f, static_cast<float>(world_width()) - 1, static_cast<float>(world_height()) - 1);
	return rect;
#else
	return Rectangle_2df(0.0f, 0.0f, static_cast<float>(world_width()) - 1, static_cast<float>(world_height()) - 1);
#endif
}


/**
 * Called by update().
 * 
 * If the camera is attached to an entity, updates the camera
 * position based on the Entity's position.
 * 
 * \note	Attempts to always stay centered on the attached
 *			Entity. Cannot scroll past its anchor space.
 */
void Map::updateCamera()
{
	if(mCameraFocus == NULL)
		return;

	setCamera(mCameraFocus->position().x() - mViewport.width() / 2, mCameraFocus->position().y() - mViewport.height() / 2);
}


const string& Map::exitDestination() const
{
	return mEdgeExitDestination;
}


const Point_2d& Map::exitDestinationPosition() const
{
	return mEdgeExitPosition;
}


/**
 * Indicates that the given coordinates are at the edge
 * and that there is an edge exit.
 */
bool Map::edgeExit(int x, int y) const
{
	if(mEdgeExit && isPointInRect(x, y, EDGE_MARGIN, EDGE_MARGIN, world_width() - EDGE_MARGIN * 2, world_height() - EDGE_MARGIN * 2))
		return true;

	return false;
}


/**
 * Updates the map and draws it.
 * 
 * Higher performing version of the update function that makes fewer
 * rendering decisions to improve speed.
 */
void Map::update()
{
	updateCamera();

	Renderer& r = Utility<Renderer>::get();

	int offsetX = mViewport.x() + static_cast<int>(mCameraPosition.x()) % CELL_DIMENSIONS.width();
	int offsetY = mViewport.y() + static_cast<int>(mCameraPosition.y()) % CELL_DIMENSIONS.height();

	static Point_2d tileUpperLeft;
	static Point_2d tileLowerRight;

	tileUpperLeft.x(gridLocation(static_cast<int>(mCameraPosition.x()), 0, CELL_DIMENSIONS.width(), mViewport.width()));
	tileUpperLeft.y(gridLocation(static_cast<int>(mCameraPosition.y()), 0, CELL_DIMENSIONS.height(), mViewport.height()));

	tileLowerRight.x(gridLocation(static_cast<int>(mCameraPosition.x()) + mViewport.width(), 0, CELL_DIMENSIONS.width(), mViewport.width()));
	tileLowerRight.y(gridLocation(static_cast<int>(mCameraPosition.y()) + mViewport.height(), 0, CELL_DIMENSIONS.height(), mViewport.height()));

	// The '&& x < dimension()' is a bit hackish... would like to clean this up.
	for(int row = tileUpperLeft.y(); row <= tileLowerRight.y() && row < height(); row++)
	{
		for(int col = tileUpperLeft.x(); col <= tileLowerRight.x() && col < width(); col++)
		{
			Cell cell = mField.cell(col, row);
			
			int rasterX = mViewport.x() + ((col - tileUpperLeft.x()) * mTileset.width()) - offsetX + mViewport.x();
			int rasterY = mViewport.y() + ((row - tileUpperLeft.y()) * mTileset.height()) - offsetY + mViewport.y();

			if (mDrawBg && cell.index(Cell::LAYER_BASE) >= 0)
				mTileset.drawTile(cell.index(Cell::LAYER_BASE), rasterX, rasterY);

			if(mDrawBgDetail && cell.index(Cell::LAYER_BASE_DETAIL) >= 0)
				mTileset.drawTile(cell.index(Cell::LAYER_BASE_DETAIL), rasterX, rasterY);

			if(mDrawDetail && cell.index(Cell::LAYER_DETAIL) >= 0)
				mTileset.drawTile(cell.index(Cell::LAYER_DETAIL), rasterX, rasterY);

		}
	}

	for(size_t i = 0; i < mEntityList.size(); i++)
	{
		Entity* e = mEntityList[i];
		e->update();
		e->draw(static_cast<int>(e->position().x() - mCameraPosition.x()), static_cast<int>(e->position().y() - mCameraPosition.y()));
	}

	for(int row = tileUpperLeft.y(); row <= tileLowerRight.y() && row < height(); row++)
	{
		for(int col = tileUpperLeft.x(); col <= tileLowerRight.x() && col < width(); col++)
		{
			Cell cell = mField.cell(col, row);
			
			int rasterX = mViewport.x() + ((col - tileUpperLeft.x()) * mTileset.width()) - offsetX + mViewport.x();
			int rasterY = mViewport.y() + ((row - tileUpperLeft.y()) * mTileset.height()) - offsetY + mViewport.y();

			if(mDrawForeground && cell.index(Cell::LAYER_FOREGROUND) >= 0)
				mTileset.drawTile(cell.index(Cell::LAYER_FOREGROUND), rasterX, rasterY);

			if(mDrawCollision && cell.blocked())
				r.drawBoxFilled(static_cast<float>(rasterX), static_cast<float>(rasterY), static_cast<float>(CELL_DIMENSIONS.width()), static_cast<float>(CELL_DIMENSIONS.height()), 255, 0, 0, 65);
			
			if(mShowLinks && cell.linked())
				r.drawBox(static_cast<float>(rasterX), static_cast<float>(rasterY), static_cast<float>(CELL_DIMENSIONS.width()), static_cast<float>(CELL_DIMENSIONS.height()), 255, 255, 0);

		}
	}

}


Rectangle_2d Map::injectMousePosition(const Point_2d& mouseCoords)
{
	int offsetX = mViewport.x() + static_cast<int>(mCameraPosition.x()) % CELL_DIMENSIONS.width();
	int offsetY = mViewport.y() + static_cast<int>(mCameraPosition.y()) % CELL_DIMENSIONS.height();

	int gridX = gridLocation(mouseCoords.x(), static_cast<int>(mCameraPosition.x() + mViewport.x()), CELL_DIMENSIONS.width(), mViewport.width());
	int gridY = gridLocation(mouseCoords.y(), static_cast<int>(mCameraPosition.y() + mViewport.y()), CELL_DIMENSIONS.height(), mViewport.height());


	Rectangle_2d rect = CELL_DIMENSIONS;
	rect.x((gridX * CELL_DIMENSIONS.width()) - offsetX);
	rect.y((gridY * CELL_DIMENSIONS.height()) - offsetY);
	//r.drawBox(rect, 255, 255, 255);

	return rect;
}


/**
 * 
 */
Point_2d Map::getGridCoords(const Point_2d& _pt) const
{
	return Point_2d	(
		gridLocation(_pt.x() - mViewport.x(), static_cast<int>(mCameraPosition.x()), CELL_DIMENSIONS.width(), mViewport.width()) + static_cast<int>(mCameraPosition.x()) / mTileset.width(),
		gridLocation(_pt.y() - mViewport.y(), static_cast<int>(mCameraPosition.y()), CELL_DIMENSIONS.height(), mViewport.height()) + static_cast<int>(mCameraPosition.y()) / mTileset.height()
					);
}


/**
 * Gets a Cell under a given world coordinate.
 * 
 * \note	Provides basic bounds checking.
 */
Cell& Map::getCell(const Point_2d& _pt)
{
	Point_2d pt = getGridCoords(_pt);

	pt(clamp(pt.x(), 0, width() - 1), clamp(pt.y(), 0, height() - 1));

	return mField.cell(pt.x(), pt.y());
}


/**
 * Gets a cell by the grid coordinates it occupies.
 * 
 * \warning	This function performs no error checking or correcting
 *			so asking for out-of-bound cells will result in thrown
 *			exceptions.
 */
Cell& Map::getCellByGridCoords(const Point_2d& grid)
{
	return getCellByGridCoords(grid.x(), grid.y());
}


/**
 * Gets a cell by the grid coordinates it occupies.
 * 
 * \warning	This function performs no error checking or correcting
 *			so asking for out-of-bound cells will result in thrown
 *			exceptions.
 */
Cell& Map::getCellByGridCoords(int x, int y)
{
	return mField.cell(x, y);
}


/**
 * 
 */
void Map::validateCameraPosition()
{
	mCameraPosition(clamp(mCameraPosition.x(), 0.0f, static_cast<float>(mCameraSpace.width())), clamp(mCameraPosition.y(), 0.0f, static_cast<float>(mCameraSpace.height())));

	/*
	if(mCameraPosition.x < 0.0f)
		mCameraPosition.x = 0.0f;
	if(mCameraPosition.x > mCameraSpace.w)
		mCameraPosition.x = static_cast<float>(mCameraSpace.w);

	if(mCameraPosition.y < 0.0f)
		mCameraPosition.y = 0.0f;
	if(mCameraPosition.y > mCameraSpace.h)
		mCameraPosition.y = static_cast<float>(mCameraSpace.h);
	*/
}


/**
 * Updates the camera's position.
 * 
 * \param	x	Amount to move the camera along the 'X' axis.
 * \param	y	Amount to move the camera along the 'Y' axis.
 */
void Map::moveCamera(float x, float y)
{
	mCameraPosition.x(mCameraPosition.x() + x);
	mCameraPosition.y(mCameraPosition.y() + y);

	validateCameraPosition();
}


/**
 * Sets the camera's position.
 * 
 * \param	x	X-Position to set the camera to.
 * \param	y	Y-Position to set the camera to.
 */
void Map::setCamera(float x, float y)
{
	mCameraPosition(x, y);
	validateCameraPosition();
}


void Map::load(const std::string& filepath)
{

}


void Map::parseProperties(Xml::XmlNode* node)
{

}


void Map::parseTilesets(Xml::XmlNode* node)
{

}


void Map::parseLevels(Xml::XmlNode* node)
{

}


void Map::parseObjects(Xml::XmlNode* node)
{
}


void Map::parseLinks(Xml::XmlNode* node)
{

}


void Map::save(const std::string& filePath)
{

}
