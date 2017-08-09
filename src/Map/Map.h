#ifndef __MAP__
#define __MAP__


#include "NAS2D/NAS2D.h"

#include "GameField.h"
#include "Tileset.h"

#include "Entity.h"

#include <string>

extern const std::string MAP_DRIVER_VERSION;

/**
 * \class Map
 * \brief Implements a basic 2D tile map.
 * 
 * \note	Map handles updates and drawing of any Entity objects
 *			pushed into it.
 */
class Map
{
public:

	Map(const std::string& mapPath);
	Map(const std::string& name, const std::string& tsetPath, int width, int height);

	~Map();

	const std::string& name() const;
	void name(const std::string& name);

	const std::string& bgMusic() const;
	void bgMusic(const std::string& path);

	bool showTitlePlaque() const;
	void showTitlePlaque(bool _b);

	void moveCamera(float x, float y);
	void setCamera(float x, float y);

	const Point_2df& cameraPosition() const { return mCameraPosition; }

	Point_2d getGridCoords(const Point_2d& _pt) const;

	Cell& getCell(const Point_2d& _pt);
	Cell& getCellByGridCoords(const Point_2d& grid);
	Cell& getCellByGridCoords(int x, int y);

	Tileset& tileset() { return mTileset; }

	void save(const std::string& filePath);

	void dump(const std::string& filePath);

	void viewport(const Rectangle_2d& _r);
	const Rectangle_2d viewport() const { return mViewport; }

	int width() const;
	int height() const;

	int world_width() const;
	int world_height() const;
	Rectangle_2df world_size() const;

	void cameraFocus(Entity* entity);

	void push_entity(Entity* entity);
	void pop_entity(Entity* entity);

	void update();

	bool edgeExit(int x, int y) const;
	const std::string& exitDestination() const;
	const Point_2d& exitDestinationPosition() const;

protected:

	friend class EditorState;
	friend class MiniMap;

	void drawBg(bool draw);
	void drawBgDetail(bool draw);
	void drawDetail(bool draw);
	void drawForeground(bool draw);
	void drawCollision(bool draw);

	void showLinks(bool show);

	GameField& field() { return mField; }
	void field(const GameField& field) { mField = field; }

	Rectangle_2d injectMousePosition(const Point_2d& mouseCoords);

private:

	typedef std::vector<Entity*>	EntityPtrList;

	Map();

	void load(const std::string& filepath);

	void parseProperties(NAS2D::Xml::XmlNode* node);
	void parseTilesets(NAS2D::Xml::XmlNode* node);
	void parseLevels(NAS2D::Xml::XmlNode* node);
	void parseObjects(NAS2D::Xml::XmlNode* node);
	void parseLinks(NAS2D::Xml::XmlNode* node);

	void validateCameraPosition();

	int gridLocation(int point, int cameraPoint, int tileDimension, int viewportDimension) const;

	void updateCamera();

	std::string		mName;
	std::string		mMessage;
	std::string		mBgMusic;
	std::string		mEdgeExitDestination;	/**< Edge exit destination map. */

	GameField		mField;
	Tileset			mTileset;

	Point_2d		mEdgeExitPosition;		/**< Edge exit destination position. */

	Point_2df		mCameraPosition;
	Rectangle_2d	mViewport;
	Rectangle_2d	mCameraSpace;

	Entity			*mCameraFocus;

	EntityPtrList	mEntityList;

	bool			mDrawBg;				/**< Flag indicating that the background layer should be drawn. */
	bool			mDrawBgDetail;			/**< Flag indicating that the background detail layer should be drawn. */
	bool			mDrawDetail;			/**< Flag indicating that the detail layer should be drawn. */
	bool			mDrawForeground;		/**< Flag indicating that the foreground layer should be drawn. */
	bool			mDrawCollision;			/**< Flag indicating that the map should draw the collision information. */
	bool			mShowLinks;				/**< Flag indicating that the map should highlight cells that are linked. */
	bool			mShowTitlePlaque;		/**< Flag indicating that a title plaque should be displayed for this Map. */
	bool			mEdgeExit;				/**< Flag indicating that the Map's edge is use as an exit. */
};

#endif