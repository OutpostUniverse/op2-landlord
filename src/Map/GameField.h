#ifndef __GAME_FIELD__
#define __GAME_FIELD__

#include <vector>

#include "Cell.h"


/**
 * \class GameField
 * \brief 
 * 
 * 
 */
class GameField
{
public:

	GameField();
	GameField(int width, int height);

	Cell& cell(int x, int y);

	void resize(int width, int height);

	int width() const;
	int height() const;

	bool empty() const;

private:

	std::vector<std::vector<Cell> >		mField;
};


#endif