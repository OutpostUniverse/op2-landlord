#include "ListBox.h"

#include "Common.h"


/**
 * C'tor
 */
ListBox::ListBox() :
	mText(Color::White),
	mHighlightBg(Color{0, 255, 0, 80}),
	mHighlightText(Color::White)
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &ListBox::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().connect(this, &ListBox::onMouseMove);
	Utility<EventHandler>::get().mouseWheel().connect(this, &ListBox::onMouseWheel);
	
	init();
}


/**
 * D'tor
 */
ListBox::~ListBox()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &ListBox::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &ListBox::onMouseMove);
	Utility<EventHandler>::get().mouseWheel().disconnect(this, &ListBox::onMouseWheel);

	mSlider.change().disconnect(this, &ListBox::slideChanged);
}

/**
*
*/
void ListBox::init()
{
	mSlider.length(0);
	mSlider.thumbPosition(0);
	mSlider.change().connect(this, &ListBox::slideChanged);
}


void ListBox::size(float w, float h)
{
	Control::size(w, h);
	deleteControl("mSlider");
	addControl("mSlider", &mSlider, rect().width() - 14, 0);
	mSlider.font(font());
	mSlider.displayPosition(false);
	mSlider.size(14, rect().height());
	mDisplayLines = static_cast<int>(rect().height() / mLineHeight);
	_checkSlider();
}


void ListBox::onFontChanged()
{
	mLineHeight = font().height() + 2;
	mDisplayLines = static_cast<int>(rect().height() / mLineHeight);
}


/**
 * Gets whether the menu is empty or not.
 */
bool ListBox::empty() const
{
	return mItems.empty();
}


/**
 * Adds an item to the Menu.
 *
 * \param	item	Item to add.
 *
 * \warning	Menu::font(Font& font) must have been called with a valid Font
 *			before this function can be safely called.
 *
 * \todo	Make this function safe to call regardless of whether a font
 *			has been defined or not.
 */
void ListBox::addItem(const std::string& item)
{
	mItems.push_back(item);
	sort();
	_checkSlider();
}


/**
 * Removes a named item from the Menu.
 *
 * \todo	Complete this function.
 *
 * \note	Safe to call if Menu is empty.
 */
void ListBox::removeItem(const std::string& item)
{
	// Ignore if menu is empty
	if(empty())
		return;

	StringList::iterator it = mItems.begin();

	while(it != mItems.end())
	{
		if(toLowercase((*it)) == toLowercase(item))
		{
			mItems.erase(it);
			mCurrentSelection = NO_SELECTION;
			return;
		}

		++it;
	}

	sort();
	_checkSlider();
}


bool ListBox::itemExists(const std::string& item)
{
	// Ignore if menu is empty
	if(empty())
		return false;

	for(size_t i = 0; i < mItems.size(); i++)
	{
		if(toLowercase(mItems[i]) == toLowercase(item))
			return true;
	}

	return false;
}


/**
 * Drops all items from the list.
 */
void ListBox::dropAllItems()
{
	mItems.clear();
	mCurrentSelection = 0;
	_checkSlider();
}


void ListBox::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	// Ignore if menu is empty or invisible
	if (empty() || !visible()) { return; }

	if (!rect().contains(Point<int>{x, y})) { return; }

	if (mSlider.visible() && mSlider.rect().contains(Point<int>{x, y}))
		return;		// if the mouse is on the slider then the slider should handle that

	int idx = ((y - (int)rect().y()) / (font().height() + 2)) % ((int)rect().height() / (font().height() + 2)) + mCurrentOffset;
	currentSelection(idx);
}


void ListBox::onMouseMove(int x, int y, int relX, int relY)
{
	// Ignore if menu is empty or invisible
	if (empty() || !visible()) { return; }

	mMouseCoords = {x, y};
}


void ListBox::_checkSlider()
{
	mItemMin = 0;
	mItemMax = mItems.size();

	if ((mLineHeight * mItems.size()) > rect().height())
	{
		if (mDisplayLines < static_cast<int>(mItems.size()))
		{
			mSlider.length(mItems.size() - mDisplayLines);
			mSlider.visible(true);
			mCurrentOffset = mSlider.thumbPosition();
			mItemMin = mCurrentOffset;
			mItemMax = mCurrentOffset + mDisplayLines;
			mItemWidth = rect().width() - mSlider.rect().width();
		}
	}
	else
	{
		mSlider.visible(false);
	}
}


void ListBox::update()
{
	// Ignore if menu is empty or invisible
	if (empty() || !visible()) { return; }

	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), NAS2D::Color::Black);

	// draw boundaries of the widget
	auto boxBounds = rect();
	boxBounds.width() = mItemWidth;
	r.drawBox(boxBounds, Color{0, 0, 0, 100});
	r.drawBoxFilled(boxBounds, Color{225, 225, 0, 85});

	// Highlight currently selected file
	boxBounds.height() = mLineHeight;
	if (mItemMin <= mCurrentSelection && mCurrentSelection < mItemMax)
	{
		boxBounds.y() = rect().y() + ((mCurrentSelection - mCurrentOffset)  * mLineHeight);
		r.drawBoxFilled(boxBounds, mHighlightBg);
	}

	// display actuals values that are ment to be
	for (int i = mItemMin; i < mItemMax; i++)
	{
		const auto position = Point{rect().x(),rect().y() + ((i - mItemMin) * mLineHeight)};
		r.drawTextShadow(font(), mItems[i], position, {1, 1}, mText, Color::Black);
	}

	r.drawBox(rect(), NAS2D::Color::Black);

	// draw the slider if needed
	mSlider.update();
}


void ListBox::onMouseWheel(int x, int y)
{
	if (!rect().contains(mMouseCoords)) { return; }

	if (y < 0)
	{
		mSlider.changeThumbPosition(1.0);
	}
	else if (y > 0)
	{
		mSlider.changeThumbPosition(-1.0);
	}
}


void ListBox::slideChanged(double _position)
{
	_checkSlider();

	int pos = static_cast<int>(_position);
	if (static_cast<float>(pos) != _position)
		mSlider.thumbPosition(static_cast<double>(pos));
}