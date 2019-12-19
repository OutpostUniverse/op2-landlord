#pragma once

/**
 * \class	Pattern
 * \brief	Defines a rectangular pattern with different integer properties.
 */
class Pattern
{
public:
	Pattern(int width = 1, int height = 1)
	{
		int w = width, h = height;

		// Ensure minimum dimensions.
		if(w < 1)
			w = 1;
		if(h < 1)
			h = 1;

		mPatternGrid.resize(height);

		for(size_t i = 0; i < static_cast<size_t>(height); i++)
			mPatternGrid[i].resize(width);
	}

	int width() const { return mPatternGrid[0].size(); }
	void width(int w) { size(w, height()); }

	int height() const { return mPatternGrid.size(); }
	void height(int h) { size(width(), h); }

	/**
	 * Changes the size of the pattern.
	 * 
	 * \warning	Does not check for garbage values.
	 * \warning	Do not set either parameter to 0 or negative values.
	 */
	void size(int w, int h)
	{
		mPatternGrid.resize(h);
		
		for(size_t i = 0; i < mPatternGrid.size(); i++)
			mPatternGrid[i].resize(w);
	}

	/**
	 * Gets a value out of the pattern grid at X, Y.
	 * 
	 * \note	Does not perform any bounds checking. Attempting to read
	 *			outside bounds will result in exceptions.
	 */
	int value(int x, int y) const { return mPatternGrid[y][x]; }

	
	/**
	 * Sets a value in the pattern grid at X, Y.
	 * 
	 * \note	Does not perform any bounds checking. Attempting to read
	 *			outside bounds will result in exceptions.
	 */
	void value(int x, int y, int value) { mPatternGrid[y][x] = value; }

private:
	typedef std::vector<std::vector<int> > Grid;

	Grid	mPatternGrid;
};
