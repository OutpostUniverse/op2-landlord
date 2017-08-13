#pragma once

#include "NAS2D/NAS2D.h"

#include <map>
#include <string>
#include <algorithm>

#include "UIContainer.h"
#include "Slider.h"

class ListBox: public UIContainer
{
public:
	typedef NAS2D::Signals::Signal0<void> SelectionChangedCallback;

public:
	ListBox();
	~ListBox();

	void init();
	void font(Font& font) { mFont = &font; }

	void sorted(bool _b) { mSorted = _b; }
	bool sorted(bool) const { return mSorted; }

	void sort() { std::sort(mItems.begin(), mItems.end()); }

	void textColor(const Color_4ub& color)	{ mText = color; }
	void selectColor(const Color_4ub& color)	{ mHighlightBg = color; }

	void addItem(const std::string& item);
	void removeItem(const std::string& item);
	bool itemExists(const std::string& item);
	void dropAllItems();

	int currentSelection() const { return mCurrentSelection; }
	void currentSelection(int selection) { mCurrentSelection = selection; mSelectionChanged(); }

	const std::string& selectionText() const { return mItems[mCurrentSelection]; }

	void update();
	virtual void size(float w, float h); 	/*!< Set the slider size. */

	bool empty() const;

	SelectionChangedCallback& selectionChanged() { return mSelectionChanged; }

protected:
	virtual void onMouseDown(EventHandler::MouseButton button, int x, int y) final;
	virtual void onMouseMove(int x, int y, int relX, int relY) final;
	void onMouseWheel(int x, int y);
	void slideChanged(double _position);

private:
	ListBox(const ListBox&) = delete;				/**< Not allowed */
	ListBox& operator=(const ListBox&) = delete;	/**< Not allowed */

	Font& font() { return *mFont; }

private:
	int							mCurrentSelection = NO_SELECTION;	/**< Current selection index. */
	int							mCurrentOffset = 0;					/**< Current selection index. */

	Font*						mFont = nullptr;	/**< Internal font to use for the menu. */

	StringList					mItems;				/**< List of items preserved in the order in which they're added. */

	Color_4ub					mText;				/**< Text Color */
	Color_4ub					mHighlightBg;		/**< Highlight Background color. */
	Color_4ub					mHighlightText;		/**< Text Color for an item that is currently highlighted. */

	Point_2d					mMouseCoords;		/**< Mouse position. */

	SelectionChangedCallback	mSelectionChanged;	/**< Callback for selection changed callback. */
	Slider						mSlider;			/**<  */
	
	bool						mSorted = false;	/**< Flag indicating that all Items should be sorted. */
};
