#include "GameField.h"


/**
 * C'tor
 */
GameField::GameField()
{}


/**
 * C'tor
 */
GameField::GameField(int width, int height)
{
	mField.resize(height);

	for(size_t row = 0; row < mField.size(); row++)
	{
		mField[row].resize(width);
	}
}


/**
 * Returns a reference to a Cell object given an X/Y coordinate pair.
 * 
 * \warning	There is no error checking in this function. Asking for a cell
 *			out of range will throw an exception.
 */
Cell& GameField::cell(int x, int y)
{
	return mField[y][x];
}


/**
 * Resizes a game field.
 */
void GameField::resize(int width, int height)
{
	mField.resize(height);

	for(size_t i = 0; i < mField.size(); i++)
		mField[i].resize(width);
}


/**
 * Returns the width of the game field.
 */
int GameField::width() const
{
	return mField[0].size();
}


/**
 * Returns the height of the game field.
 */
int GameField::height() const
{
	return mField.size();
}


/**
 * Gets whether the GameField is empty or not.
 */
bool GameField::empty() const
{
	return mField.empty();
}