#include "Map.h"

#include "../Common.h"

#include <cmath>
#include <sstream>

using namespace std;

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
																				mTileset(tsetPath, CELL_DIMENSIONS.w(), CELL_DIMENSIONS.h()),
																				mViewport(0, 0, static_cast<int>(Utility<Renderer>::get().width()), static_cast<int>(Utility<Renderer>::get().height())),
																				mCameraSpace(0, 0, mField.width() * mTileset.width() - mViewport.w(), mField.height() * mTileset.height() - mViewport.h()),
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
	mCameraSpace(0, 0, (width() * mTileset.width()) - mViewport.w(), (height() * mTileset.height()) - mViewport.h());
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
	return mField.width() * CELL_DIMENSIONS.w();
}


/**
 * Gets the map's world height.
 */
int Map::world_height() const
{
	return mField.height() * CELL_DIMENSIONS.h();
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

	setCamera(mCameraFocus->position().x() - mViewport.w() / 2, mCameraFocus->position().y() - mViewport.h() / 2);
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

	int offsetX = mViewport.x() + static_cast<int>(mCameraPosition.x()) % CELL_DIMENSIONS.w();
	int offsetY = mViewport.y() + static_cast<int>(mCameraPosition.y()) % CELL_DIMENSIONS.h();

	static Point_2d tileUpperLeft;
	static Point_2d tileLowerRight;

	tileUpperLeft.x(gridLocation(static_cast<int>(mCameraPosition.x()), 0, CELL_DIMENSIONS.w(), mViewport.w()));
	tileUpperLeft.y(gridLocation(static_cast<int>(mCameraPosition.y()), 0, CELL_DIMENSIONS.h(), mViewport.h()));

	tileLowerRight.x(gridLocation(static_cast<int>(mCameraPosition.x()) + mViewport.w(), 0, CELL_DIMENSIONS.w(), mViewport.w()));
	tileLowerRight.y(gridLocation(static_cast<int>(mCameraPosition.y()) + mViewport.h(), 0, CELL_DIMENSIONS.h(), mViewport.h()));

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
				r.drawBoxFilled(static_cast<float>(rasterX), static_cast<float>(rasterY), static_cast<float>(CELL_DIMENSIONS.w()), static_cast<float>(CELL_DIMENSIONS.h()), 255, 0, 0, 65);
			
			if(mShowLinks && cell.linked())
				r.drawBox(static_cast<float>(rasterX), static_cast<float>(rasterY), static_cast<float>(CELL_DIMENSIONS.w()), static_cast<float>(CELL_DIMENSIONS.h()), 255, 255, 0);

		}
	}

}


Rectangle_2d Map::injectMousePosition(const Point_2d& mouseCoords)
{
	int offsetX = mViewport.x() + static_cast<int>(mCameraPosition.x()) % CELL_DIMENSIONS.w();
	int offsetY = mViewport.y() + static_cast<int>(mCameraPosition.y()) % CELL_DIMENSIONS.h();

	int gridX = gridLocation(mouseCoords.x(), static_cast<int>(mCameraPosition.x() + mViewport.x()), CELL_DIMENSIONS.w(), mViewport.w());
	int gridY = gridLocation(mouseCoords.y(), static_cast<int>(mCameraPosition.y() + mViewport.y()), CELL_DIMENSIONS.h(), mViewport.h());


	Rectangle_2d rect = CELL_DIMENSIONS;
	rect.x((gridX * CELL_DIMENSIONS.w()) - offsetX);
	rect.y((gridY * CELL_DIMENSIONS.h()) - offsetY);
	//r.drawBox(rect, 255, 255, 255);

	return rect;
}


/**
 * 
 */
Point_2d Map::getGridCoords(const Point_2d& _pt) const
{
	return Point_2d	(
		gridLocation(_pt.x() - mViewport.x(), static_cast<int>(mCameraPosition.x()), CELL_DIMENSIONS.w(), mViewport.w()) + static_cast<int>(mCameraPosition.x()) / mTileset.width(),
		gridLocation(_pt.y() - mViewport.y(), static_cast<int>(mCameraPosition.y()), CELL_DIMENSIONS.h(), mViewport.h()) + static_cast<int>(mCameraPosition.y()) / mTileset.height()
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
	mCameraPosition(clamp(mCameraPosition.x(), 0.0f, static_cast<float>(mCameraSpace.w())), clamp(mCameraPosition.y(), 0.0f, static_cast<float>(mCameraSpace.h())));

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
	File xmlFile = Utility<Filesystem>::get().open(filepath);

	TiXmlDocument doc;
	TiXmlElement  *root;

	// Load the XML document and handle any errors if occuring
	doc.Parse(xmlFile.raw_bytes());
	if(doc.Error())
	{
		cout << "Malformed map file. Error on Row " << doc.ErrorRow() << ", Column " << doc.ErrorCol() << ": " << doc.ErrorDesc() << endl;
		return;
	}
	else
	{
		// Find the 'map' tag
		root = doc.FirstChildElement("map");
		if(root == 0)
		{
			cout << "Root element in '" << filepath << "' is not 'map'." << endl;
			return;
		}

		// Start parsing through the Config.xml file.
		int result = 0;
		
		TiXmlNode* node = 0;
		while(node = root->IterateChildren(node))
		{
			if(node->ValueStr() == "properties")
				parseProperties(node);
			else if(node->ValueStr() == "tilesets")
				parseTilesets(node);
			else if(node->ValueStr() == "levels")
				parseLevels(node);
			else if(node->ValueStr() == "objects")
				parseObjects(node);
			else if(node->ValueStr() == "links")
				parseLinks(node);
			else
				cout << "Unexpected tag '<" << node->ValueStr() << ">' found in '" << filepath << "' on row " << node->Row() << "." << endl;
		}

		mCameraSpace = Rectangle_2d(0, 0, mField.width() * mTileset.width() - mViewport.w(), mField.height() * mTileset.height() - mViewport.h());
		//mFieldLoops = Point_2d(mViewport.w / mTileset.width() + 1, mViewport.h / mTileset.height() + 1);
	}
}


void Map::parseProperties(TiXmlNode* node)
{
	XmlAttributeParser parser;

	TiXmlNode *xmlNode = 0;
	while(xmlNode = node->IterateChildren(xmlNode))
	{
		if(xmlNode->ValueStr() == "mapname")
		{
			name(parser.stringAttribute(xmlNode, "name"));
		}
		else if(xmlNode->ValueStr() == "mapsize")
		{
			int width = parser.intAttribute(xmlNode, "width");
			int height = parser.intAttribute(xmlNode, "height");

			mField.resize(width, height);
		}
		else if(xmlNode->ValueStr() == "tilesize")
		{
			int width = parser.intAttribute(xmlNode, "width");
			int height = parser.intAttribute(xmlNode, "height");

			if(width != CELL_DIMENSIONS.w() || height != CELL_DIMENSIONS.h())
				cout << "Tile sizes other than " << CELL_DIMENSIONS.w() << "x" << CELL_DIMENSIONS.h() << " pixels not supported." << endl;
		}
		else if(xmlNode->ValueStr() == "bg_music")
		{
			bgMusic(parser.stringAttribute(xmlNode, "path"));
		}
		else if(xmlNode->ValueStr() == "title_plaque")
		{
			toLowercase(parser.stringAttribute(xmlNode, "show")) == "true" ? showTitlePlaque(true) : showTitlePlaque(false);
		}
		else if(xmlNode->ValueStr() == "edge_exit")
		{
			mEdgeExitDestination = parser.stringAttribute(xmlNode, "destination");
			mEdgeExitPosition(parser.intAttribute(xmlNode, "dest_x"), parser.intAttribute(xmlNode, "dest_y"));
			mEdgeExit = true;
		}
		else
			cout << "Unexpected tag '<" << xmlNode->ValueStr() << ">' found in map file on row " << xmlNode->Row() << "." << endl;
	}
}


void Map::parseTilesets(TiXmlNode* node)
{
	XmlAttributeParser parser;

	TiXmlNode *xmlNode = 0;
	while(xmlNode = node->IterateChildren(xmlNode))
	{
		if(xmlNode->ValueStr() == "tileset")
		{
			string tsetpath = parser.stringAttribute(xmlNode, "path");

			if (!Utility<Filesystem>::get().exists(tsetpath))
				throw Exception(0, "Missing TileSet", "Referened TileSet missing: " + tsetpath);

			mTileset = Tileset(tsetpath, CELL_DIMENSIONS.w(), CELL_DIMENSIONS.h());
		}
		else
			cout << "Unexpected tag '<" << xmlNode->ValueStr() << ">' found in map file on row " << xmlNode->Row() << "." << endl;
	}
}


void Map::parseLevels(TiXmlNode* node)
{
	XmlAttributeParser parser;

	TiXmlNode *xmlNode = 0;
	while(xmlNode = node->IterateChildren(xmlNode))
	{
		if(xmlNode->ValueStr() == "level")
		{
			int cellCounter = 0;
			int w = mField.width();
			
			TiXmlNode* cellNode = 0;
			while(cellNode = xmlNode->IterateChildren(cellNode))
			{
				int bg_index = parser.intAttribute(cellNode, "bg_index");
				int bgdetail_index = parser.intAttribute(cellNode, "bgd_index");
				int detail_index = parser.intAttribute(cellNode, "d_index");
				int fg_index = parser.intAttribute(cellNode, "fg_index");
				string blocked = toLowercase(parser.stringAttribute(cellNode, "blocked"));

				int col = cellCounter % w;
				int row = cellCounter / w;
				mField.cell(col, row).index(Cell::LAYER_BASE, bg_index);
				mField.cell(col, row).index(Cell::LAYER_BASE_DETAIL, bgdetail_index);
				mField.cell(col, row).index(Cell::LAYER_DETAIL, detail_index);
				mField.cell(col, row).index(Cell::LAYER_FOREGROUND, fg_index);
				
				blocked == "true" ? mField.cell(col, row).blocked(true) : mField.cell(col, row).blocked(false) ;

				cellCounter++;
			}

			if(cellCounter < (mField.width() * mField.height()))
				cout << "WARNING: Map doesn't define enough cells." << endl;
			else if(cellCounter > (mField.width() * mField.height()))
				cout << "WARNING: Map defines to many cells." << endl;
		}
		else
			cout << "Unexpected tag '<" << xmlNode->ValueStr() << ">' found in map file on row " << xmlNode->Row() << "." << endl;
	}
}


void Map::parseObjects(TiXmlNode* node)
{
}


void Map::parseLinks(TiXmlNode* node)
{
	XmlAttributeParser parser;

	if(mField.empty())
	{
		cout << "WARNING: Links section defined before levels or no cells were defined. Links will be ignored." << endl;
		return;
	}


	TiXmlNode *xmlNode = 0;
	while(xmlNode = node->IterateChildren(xmlNode))
	{
		if(xmlNode->ValueStr() == "link")
		{
			std::string destination = parser.stringAttribute(xmlNode, "destination");
			int dest_x = parser.intAttribute(xmlNode, "dest_x");
			int dest_y = parser.intAttribute(xmlNode, "dest_y");
			int row = parser.intAttribute(xmlNode, "row");
			int col = parser.intAttribute(xmlNode, "col");

			Cell& _c = mField.cell(row, col);
			_c.link(destination);
			_c.link_destination(Point_2d(dest_x, dest_y));
		}
		else
			cout << "Unexpected tag '<" << xmlNode->ValueStr() << ">' found in map file on row " << xmlNode->Row() << "." << endl;
	}
}


void Map::save(const std::string& filePath)
{
	TiXmlDocument doc;

	//TiXmlComment *comment = new TiXmlComment("Automatically generated Configuration file. This is best left untouched.");
	//doc->LinkEndChild(comment);

	TiXmlElement* root = new TiXmlElement("map");
	root->SetAttribute("version", MAP_DRIVER_VERSION);
	doc.LinkEndChild(root);

	
	// ==========================================
	// MAP PROPERTIES
	// ==========================================
	TiXmlElement *properties = new TiXmlElement("properties");
	root->LinkEndChild(properties);

	TiXmlElement *mapname = new TiXmlElement("mapname");
	mapname->SetAttribute("name", mName);
	properties->LinkEndChild(mapname);

	TiXmlElement *mapsize = new TiXmlElement("mapsize");
	mapsize->SetAttribute("width", mField.width());
	mapsize->SetAttribute("height", mField.height());
	properties->LinkEndChild(mapsize);

	TiXmlElement *bg_music = new TiXmlElement("bg_music");
	bg_music->SetAttribute("path", mBgMusic);
	properties->LinkEndChild(bg_music);

	TiXmlElement *title_plaque = new TiXmlElement("title_plaque");
	showTitlePlaque() ? title_plaque->SetAttribute("show", "true") : title_plaque->SetAttribute("show", "false");
	properties->LinkEndChild(title_plaque);

	TiXmlElement *tilesize = new TiXmlElement("tilesize");
	tilesize->SetAttribute("width", CELL_DIMENSIONS.w());
	tilesize->SetAttribute("height", CELL_DIMENSIONS.h());
	properties->LinkEndChild(tilesize);

	if(mEdgeExit)
	{
		TiXmlElement *edge_exit = new TiXmlElement("edge_exit");
		edge_exit->SetAttribute("destination", mEdgeExitDestination);
		edge_exit->SetAttribute("dest_x", mEdgeExitPosition.x());
		edge_exit->SetAttribute("dest_y", mEdgeExitPosition.y());
		properties->LinkEndChild(edge_exit);
	}


	// ==========================================
	// TILE SETS
	// ==========================================
	TiXmlElement *tilesets = new TiXmlElement("tilesets");
	root->LinkEndChild(tilesets);

	TiXmlElement *tileset = new TiXmlElement("tileset");
	tileset->SetAttribute("path", mTileset.filepath());
	tilesets->LinkEndChild(tileset);


	// ==========================================
	// LEVELS
	// ==========================================
	TiXmlElement *levels = new TiXmlElement("levels");
	root->LinkEndChild(levels);

	TiXmlElement *level = new TiXmlElement("level");
	level->SetAttribute("id", 0);
	level->SetAttribute("encoding", "TEXT");
	levels->LinkEndChild(level);

	for(int row = 0; row < mField.height(); row++)
	{
		for(int col = 0; col < mField.width(); col++)
		{
			Cell& _cell = mField.cell(col, row);
			TiXmlElement *cell = new TiXmlElement("cell");

			cell->SetAttribute("bgtset", 0);
			cell->SetAttribute("bg_index", _cell.index(Cell::LAYER_BASE));
			cell->SetAttribute("bgd_tset", 0);
			cell->SetAttribute("bgd_index", _cell.index(Cell::LAYER_BASE_DETAIL));
			cell->SetAttribute("d_tset", 0);
			cell->SetAttribute("d_index", _cell.index(Cell::LAYER_DETAIL));
			cell->SetAttribute("fg_tset", 0);
			cell->SetAttribute("fg_index", _cell.index(Cell::LAYER_FOREGROUND));
			_cell.blocked() ? cell->SetAttribute("blocked", "true") : cell->SetAttribute("blocked", "false");

			level->LinkEndChild(cell);
		}
	}


	// ==========================================
	// OBJECTS
	// ==========================================
	TiXmlElement *objects = new TiXmlElement("objects");
	root->LinkEndChild(objects);


	// ==========================================
	// MAP LINKS
	// ==========================================
	TiXmlElement *links = new TiXmlElement("links");
	root->LinkEndChild(links);

	for(int col = 0; col < mField.height(); col++)
	{
		for(int row = 0; row < mField.width(); row++)
		{
			if(mField.cell(row, col).linked())
			{
				Cell& _c = mField.cell(row, col);

				TiXmlElement* link = new TiXmlElement("link");

				link->SetAttribute("row", row);
				link->SetAttribute("col", col);
				link->SetAttribute("destination", _c.link());
				link->SetAttribute("dest_x", _c.link_destination().x());
				link->SetAttribute("dest_y", _c.link_destination().y());

				links->LinkEndChild(link);
			}
		}
	}


	// Write out the XML file.
	TiXmlPrinter printer;
	doc.Accept(&printer);

	Utility<Filesystem>::get().write(File(printer.Str(), filePath));
}


/**
 * Dumps the whole map to a BMP file.
 * 
 * \note	This code uses a number of hacks provided by
 *			SDL to save an image to disk. It may be better
 *			to move some of this code over into Image so
 *			that this feature could be usable in NAS2D.
 * 
 * \warning	This function makes no attempt to trap memory
 * 			allocation errors. Should an allocation error
 * 			occur, this function will cause a crash.
 * 
 * \todo	Add memory allocation error traps to prevent
 *			possible crashing.
 * 
 * \todo	Ensure that there are no memory leaks associated
 *			with this function.
 */
void Map::dump(const std::string& filePath)
{
	/*
	Image img(mField.width() * CELL_DIMENSIONS.w, mField.height() * CELL_DIMENSIONS.h);

	for(int row = 0; row < mField.height(); row++)
	{
		for(int col = 0; col < mField.width(); col++)
		{
			Cell cell = mField.cell(col, row);

			mTileset.drawTileToImage(img, cell.index(Cell::LAYER_BASE), col * CELL_DIMENSIONS.w, row * CELL_DIMENSIONS.h);
			mTileset.drawTileToImage(img, cell.index(Cell::LAYER_BASE_DETAIL), col * CELL_DIMENSIONS.w, row * CELL_DIMENSIONS.h);
			mTileset.drawTileToImage(img, cell.index(Cell::LAYER_DETAIL), col * CELL_DIMENSIONS.w, row * CELL_DIMENSIONS.h);
			mTileset.drawTileToImage(img, cell.index(Cell::LAYER_FOREGROUND), col * CELL_DIMENSIONS.w, row * CELL_DIMENSIONS.h);
		}
	}

	SDL_Surface* surface = img.pixels();
	SDL_RWops* rw = SDL_RWFromMem(surface->pixels, surface->pitch * surface->h);

	SDL_SaveBMP_RW(surface, rw, 0);

	string bytestream;

	Uint8* pt = rw->hidden.mem.base;
	while(pt != rw->hidden.mem.stop)
	{
		bytestream.append((char*)pt, 1);
		pt++;
	}
	SDL_FreeRW(rw);

	File file(bytestream, filePath);
	Utility<Filesystem>::get().write(file);
	*/
}


