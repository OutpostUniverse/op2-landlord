#include "Entity.h"

using namespace std;

/**
 * C'tor
 * 
 * \param	name		Unique string identifier.
 * \param	obstruction	Flag indicating that the Entity is an obstruction to other Entities.
 */
Entity::Entity(const string& name, bool obstruction):	mName(name),
														mSprite(NULL),
														mObstruction(obstruction)
{
}


/**
 * D'tor
 */
Entity::~Entity()
{
	if(mSprite)
	{
		delete mSprite;
		mSprite = NULL;
	}
}


Entity::Entity(const Entity& _e):	mName(_e.mName),
									mPosition(_e.mPosition),
									mPreviousPosition(_e.mPreviousPosition),
									mArea(_e.mArea),
									mBoundingBox(_e.mBoundingBox),
									mSprite(new Sprite(*_e.mSprite)),
									mObstruction(_e.mObstruction)
{}


const Entity& Entity::operator=(const Entity& _e)
{
	mName				= _e.mName;
	mPosition			= _e.mPosition;
	mPreviousPosition	= _e.mPreviousPosition;
	mArea				= _e.mArea;
	mBoundingBox		= _e.mBoundingBox;

	if(mSprite)
		delete mSprite;

	mSprite				= new Sprite(*_e.mSprite);

	mObstruction		= _e.mObstruction;

	return *this;
}


/**
 * Sets a bounding box for the Entity.
 */
void Entity::boundingBox(const Rectangle_2df& rect)
{
	mBoundingBox = rect;
}


/**
 * Sets a bounding box for the Entity.
 */
void Entity::boundingBox(float x, float y, float width, float height)
{
	mBoundingBox(x, y, width, height);
}


void Entity::sprite(const std::string& path)
{
	if(mSprite)
	{
		delete mSprite;
		mSprite = NULL;
	}

	mSprite = new Sprite(path);
}


void Entity::draw(float x, float y)
{
	if(mSprite)
		mSprite->update(x, y);

	/*
	if(!mBoundingBox.null())
	{
		Utility<Renderer>::get().drawBox(x + mBoundingBox.x, y + mBoundingBox.y, mBoundingBox.w, mBoundingBox.h, 255, 255, 255);
	}
	*/
}