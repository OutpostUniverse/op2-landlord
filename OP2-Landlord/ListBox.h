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
	typedef NAS2D::Signal<> SelectionChangedCallback;

public:
	ListBox();
	~ListBox();

	void init();

	void sorted(bool _b) { mSorted = _b; }
	bool sorted(bool) const { return mSorted; }

	void sort() { std::sort(mItems.begin(), mItems.end()); }

	void textColor(const NAS2D::Color& color)	{ mText = color; }
	void selectColor(const NAS2D::Color& color)	{ mHighlightBg = color; }

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
	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) final;
	virtual void onMouseMove(NAS2D::Point<int> position, NAS2D::Vector<int> change) final;
	void onMouseWheel(NAS2D::Vector<int> change);
	void slideChanged(double _position);

	virtual void onFontChanged();

private:
	ListBox(const ListBox&) = delete;				/**< Not allowed */
	ListBox& operator=(const ListBox&) = delete;	/**< Not allowed */

	void _checkSlider();

private:
	int							mCurrentSelection = NO_SELECTION;	/**< Current selection index. */
	int							mCurrentOffset = 0;					/**< Current selection index. */
	int							mDisplayLines = 0;					/**< Number of lines able to be displayed. */
	int							mLineHeight = 0;					/**< Height of an individual line. */

	int							mItemMin = 0;
	int							mItemMax = 0;
	int							mItemWidth = 0;

	StringList					mItems;				/**< List of items preserved in the order in which they're added. */

	NAS2D::Color					mText;				/**< Text Color */
	NAS2D::Color					mHighlightBg;		/**< Highlight Background color. */
	NAS2D::Color					mHighlightText;		/**< Text Color for an item that is currently highlighted. */

	NAS2D::Point<int>					mMouseCoords;		/**< Mouse position. */

	SelectionChangedCallback	mSelectionChanged;	/**< Callback for selection changed callback. */
	Slider						mSlider;			/**<  */
	
	bool						mSorted = false;	/**< Flag indicating that all Items should be sorted. */
};
