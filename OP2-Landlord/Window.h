#pragma once

#include "UIContainer.h"


class Window : public UIContainer
{
public:
	Window();
	virtual ~Window();

	bool dragging() const { return mIsWindowDragging; }

	virtual bool responding_to_events() const { return dragging(); }

	void titleFont(const Font& font);
	int titleBarHeight() const;

	virtual void update();

protected:
	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) final;
	virtual void onMouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) final;
	virtual void onMouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change) final;

	virtual void draw() = 0;

	virtual void mouseDown(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) {};
	virtual void mouseUp(NAS2D::EventHandler::MouseButton button, NAS2D::Point<int> position) {};
	virtual void mouseMotion(NAS2D::Point<int> position, NAS2D::Vector<int> change) {};

	Point<int>& _mouseCoords() { return mMouseCoords; }

private:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

private:
	const Font*		mTitleFont = nullptr;

	Point<int>	mMouseCoords;

	bool		mIsWindowDragging = false;
};