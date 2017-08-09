#ifndef __ENTITY__
#define __ENTITY__

#include "NAS2D/NAS2D.h"

#include <string>

using namespace NAS2D;

/**
 * \class	Entity
 * \brief	Represents a base Entity.
 * 
 * Base class for all other Entity objects. Derived objects should override void update()
 * in order to perform unique logic every frame.
 * 
 * Entities are differntiated by their names so all Entities must have a unique name. Names
 * are case sensitive so "TREE", "tree" and "Tree" are all considered unique names.
 * 
 * 
 */
class Entity
{
public:

	Entity(const std::string& name, bool obstruction = true);
	Entity(const Entity& _e);
	virtual ~Entity();

	bool operator==(const Entity& ent) { return mName == ent.name(); }

	const Entity& operator=(const Entity& _e);

	/**
	 * Updates logic.
	 * 
	 * Pure virtual. Must be overridden in all derived types.
	 * 
	 * \note	Currently not pure virtual to help with prototyping.
	 */
	virtual void update() {};

	/**
	 * Gets whether the Entity creates an obstruction.
	 */
	bool obstruction() const { return mObstruction; }
	
	/**
	 * Gets the name of the Entity.
	 */
	const std::string& name() const { return mName; }

	/**
	 * Gets the previous position of the Entity.
	 */
	const Point_2df& prev_position() const { return mPreviousPosition; }

	/**
	 * Gets the current position of the Entity.
	 */
	const Point_2df& position() const { return mPosition; }
	
	/**
	 * Sets the current position of the Entity.
	 */
	void position(float x, float y)
	{
		mPosition(x, y);
		updatePositionRect();
	}

	/**
	 * Sets the current and previous position of the Entity.
	 */
	void init_position(float x, float y)
	{
		mPosition(x, y);
		mPreviousPosition = mPosition;
		updatePositionRect();
	}

	/**
	 * Sets the current position of the Entity.
	 */
	void position(const Point_2df& pos)
	{
		mPosition = pos;
		updatePositionRect();
	}

	/**
	 * Move's the Entity's position by X, Y.
	 */
	void move(float x, float y)
	{
		mPreviousPosition = mPosition;
		mPosition(mPosition.x() + x, mPosition.y() + y);
		validateX();
		validateY();
		updatePositionRect();
	}

	/**
	 * Move's the Entity's X-position by X.
	 */
	void move_x(float x)
	{
		mPreviousPosition.x(mPosition.x());
		mPosition.x() += x;
		validateX();
		updatePositionRect();
	}

	/**
	 * Move's the Entity's Y-position by Y.
	 */
	void move_y(float y)
	{
		mPreviousPosition.y(mPosition.y());
		//mPosition.y(mPosition.y() + y);
		mPosition.y() += y;
		validateY();
		updatePositionRect();
	}

	/**
	 * Sets the area that an Entity may exist in.
	 */
	void area(const Rectangle_2df& area) { mArea = area; }

	//bool moved() const { return mPosition != mPreviousPosition; }

	/**
	 * 
	 */
	void sprite(const std::string& path);
	Sprite* sprite() { return mSprite; }

	/**
	 * Tells the Entity to draw itself at a given screen coordinate.
	 */
	virtual void draw(float x, float y);

	void boundingBox(const Rectangle_2df& rect);
	void boundingBox(float x, float y, float width, float height);

	const Rectangle_2d& positionRect() const { return mPositionRect; }

protected:



private:

	void validateX()
	{
		mPosition.x(clamp(mPosition.x(), 0.0f, mArea.width()));
	}

	void validateY()
	{
		mPosition.y(clamp(mPosition.y(), 0.0f, mArea.height()));
	}

	/**
	 * Updates the position rectangle.
	 */
	void updatePositionRect()
	{
		mPositionRect(static_cast<int>(mPosition.x() + mBoundingBox.x()), static_cast<int>(mPosition.y() + mBoundingBox.y()), static_cast<int>(mBoundingBox.width()), static_cast<int>(mBoundingBox.height()));
	}

	Entity();	/**< Intentionally undefined. */

	std::string		mName;				/**< Name of the Entity. */

	Point_2df		mPosition;			/**< Entity's current position in the world. */
	Point_2df		mPreviousPosition;	/**< Entity's previous position in the world. */

	Rectangle_2df	mArea;				/**< Area an Entity may travel in. */
	Rectangle_2df	mBoundingBox;		/**< Axis-aligned bounding box. Defaults to nothing. */
	Rectangle_2d	mPositionRect;		/**< Current rect position. */

	Sprite			*mSprite;			/**< Sprite for Entity visuals. */

	bool			mObstruction;		/**< Flag indicating that this Entity is an obstruction. Defaults to True */
};

#endif //__ENTITY__