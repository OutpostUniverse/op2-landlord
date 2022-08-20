#pragma once

#include "UIContainer.h"


class Window : public UIContainer
{
public:
	Window();
	virtual ~Window();

	bool dragging() const { return mDragging; }

	virtual bool responding_to_events() const { return dragging(); }

	void titleFont(const Font& font);
	int titleBarHeight() const;

	virtual void update();

protected:
	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) final;
	virtual void onMouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) final;
	virtual void onMouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change) final;

	virtual void draw() = 0;	/**< Derived types must override this. */

	virtual void mouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) {};
	virtual void mouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) {};
	virtual void mouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change) {};

	Point<int>& _mouseCoords() { return mMouseCoords; }	/**< Internal function for derived types. */

private:
	Window(const Window&) = delete;				/**< Not allowed */
	Window& operator=(const Window&) = delete;	/**< Not allowed */

private:
	const Font*		mBoldFont = nullptr;			/**< Font used for window title. */

	Point<int>	mMouseCoords;					/**<  */

	bool		mDragging = false;				/**< Window is being dragged. */
};