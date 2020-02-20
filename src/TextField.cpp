// ==================================================================================
// = NAS2D+GUI
// = Copyright © 2008 - 2012, New Age Software
// ==================================================================================
// = This file is part of the NAS2D+GUI library.
// ==================================================================================

#include "TextField.h"

#include <locale>


const int FIELD_PADDING = 4;

const int CURSOR_BLINK_DELAY = 250;

std::locale LOC;

TextField::TextField():	mCursorPosition(0),
						mCursorX(0),
						mScrollOffset(0),
						mMaxCharacters(0),
						mBorderVisibility(FOCUS_ONLY),
						mEditable(true),
						mShowCursor(false),
						mNumbersOnly(false)
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &TextField::onMouseDown);
	Utility<EventHandler>::get().keyDown().connect(this, &TextField::onKeyDown);
	Utility<EventHandler>::get().textInput().connect(this, &TextField::onTextInput);
	hasFocus(false);
	Utility<EventHandler>::get().textInputMode(true);
}


TextField::~TextField()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &TextField::onMouseDown);
	Utility<EventHandler>::get().keyDown().disconnect(this, &TextField::onKeyDown);
	Utility<EventHandler>::get().textInput().disconnect(this, &TextField::onTextInput);
}

void TextField::resetCursorPosition()
{
	mCursorPosition = 0;
}


/**
 * When set, will only allow numbers to be entered into the TextField.
 * 
 * \param _b True or False.
 */
void TextField::numbers_only(bool _b)
{
	mNumbersOnly = _b;
}



/**
 * Sets the maximum number of characters allowed in the text field.
 * 
 * \param	Count	Number of characters allowed in the field.
 * 
 * \note	Calling this with \c 0 will clear character limit.
 */
void TextField::maxCharacters(size_t count)
{
	mMaxCharacters = count;
}


void TextField::onFontChanged()
{
	height(static_cast<float>(font().height() + FIELD_PADDING * 2));
}


int TextField::textAreaWidth() const
{
	return static_cast<int>(rect().width()) - FIELD_PADDING * 2;
}


void TextField::editable(bool editable)
{
	mEditable = editable;
}


bool TextField::editable() const
{
	return mEditable;
}


/**
 * Sets border visibility.
 */
void TextField::border(BorderVisibility visibility)
{
	mBorderVisibility = visibility;
}


void TextField::draw()
{
	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), 255, 255, 255);

	if (hasFocus())
	{
		r.drawBox(rect(), 0, 180, 255);
	}
	else
	{
		r.drawBox(rect(), 0, 0, 0);
	}



	if (highlight())
	{
		r.drawBox(rect(), 255, 255, 0);
	}

	drawCursor();

	if (fontSet())
	{
		r.drawText(font(), text(), positionX() + FIELD_PADDING, positionY() + FIELD_PADDING, 0, 0, 0);
	}
}


/**
 * Draws the insertion point cursor.
 */
void TextField::drawCursor()
{
	if(hasFocus())
	{
		if(mShowCursor)
		{
			// updateCursor() should be called only on events relating to the cursor so this is temporary.
			updateCursor();
			Utility<Renderer>::get().drawLine(static_cast<float>(mCursorX + 1), rect().y() + FIELD_PADDING + 1, static_cast<float>(mCursorX + 1), rect().y() + rect().height() - FIELD_PADDING, 0, 0, 0);
			Utility<Renderer>::get().drawLine(static_cast<float>(mCursorX), rect().y() + FIELD_PADDING, static_cast<float>(mCursorX), rect().y() + rect().height() - FIELD_PADDING - 1, 255, 255, 255);
		}
		
		if(mCursorTimer.accumulator() > CURSOR_BLINK_DELAY)
		{
			mCursorTimer.reset();
			mShowCursor = !mShowCursor;
		}
	}
}


/**
 * Draws a highlight over selected text.
 */
void TextField::drawTextHighlight()
{
	Utility<Renderer>::get().drawBoxFilled(rect().x() + FIELD_PADDING, rect().y(), static_cast<float>(font().width(text())), rect().height(), 0, 0, 150, 100);
}


void TextField::updateCursor()
{
	int cursorX = font().width(text().substr(0, mCursorPosition));

	if (cursorX - mScrollOffset >= textAreaWidth())
	{
		mScrollOffset = cursorX - textAreaWidth();
	}

	if (cursorX - mScrollOffset <= 0)
	{
		mScrollOffset = cursorX - textAreaWidth() / 2;
	}

	if (mScrollOffset < 0)
	{
		mScrollOffset = 0;
	}

	mCursorX = static_cast<int>(rect().x() + FIELD_PADDING + cursorX - mScrollOffset);
}


void TextField::update()
{
	if (!visible()) { return; }

	draw();
}


/**
 * Handles text input events.
 */
void TextField::onTextInput(const std::string& _s)
{
	if (!hasFocus() || !visible() || !editable() || _s.empty()) { return; }
	if (mMaxCharacters > 0 && text().length() == mMaxCharacters) { return; }

	int prvLen = text().length();

	if (mNumbersOnly && !std::isdigit(_s[0], LOC)) { return; }

	_text() = _text().insert(mCursorPosition, _s);

	if (text().length() - prvLen)
	{
		onTextChanged();
		mCursorPosition++;
	}
}


void TextField::onKeyDown(EventHandler::KeyCode key, EventHandler::KeyModifier mod, bool repeat)
{
	if (!hasFocus() || !editable() || !visible())
		return;

	switch(key)
	{	
		// COMMAND KEYS
	case EventHandler::KeyCode::KEY_BACKSPACE:
			if(!text().empty() && mCursorPosition > 0)
			{
				mCursorPosition--;
				_text().erase(mCursorPosition, 1);
				onTextChanged();
			}
			break;

		case EventHandler::KeyCode::KEY_HOME:
			mCursorPosition = 0;
			break;

		case EventHandler::KeyCode::KEY_END:
			mCursorPosition = text().length();
			break;

		case EventHandler::KeyCode::KEY_DELETE:
			if (text().length() > 0)
			{
				_text() = _text().erase(mCursorPosition, 1);
				onTextChanged();
			}
			break;

		// ARROW KEYS
		case EventHandler::KeyCode::KEY_LEFT:
			if(mCursorPosition > 0)
				--mCursorPosition;
			break;

		case EventHandler::KeyCode::KEY_RIGHT:
			if(static_cast<size_t>(mCursorPosition) < text().length())
				++mCursorPosition;
			break;

		// KEYPAD ARROWS
		case EventHandler::KeyCode::KEY_KP4:
			if((mCursorPosition > 0) && !Utility<EventHandler>::get().query_numlock())
				--mCursorPosition;
			break;

		case EventHandler::KeyCode::KEY_KP6:
			if((static_cast<size_t>(mCursorPosition) < text().length()) && !Utility<EventHandler>::get().query_numlock())
				++mCursorPosition;
			break;

		// IGNORE ENTER/RETURN KEY
		case EventHandler::KeyCode::KEY_ENTER:
		case EventHandler::KeyCode::KEY_KP_ENTER:
			break;

		// REGULAR KEYS
		default:
			break;
	}
}


/**
 * Mouse down even handler.
 */
void TextField::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	// If font is not available, back out now to prevent issues.
	if(!fontSet())
		return;

	if(!rect().contains(Point_2d(x, y)))
	{
		hasFocus(false);
		return;
	}
	else
		hasFocus(true);

	int relativePosition = static_cast<int>(x - rect().x());

	// If the click occured past the width of the text, we can immediatly
	// set the position to the end and move on.
	if(font().width(text()) < relativePosition)
	{
		mCursorPosition = text().size();
		return;
	}


	// Figure out where the click occured within the visible string.
	int i = 0;
	while(static_cast<size_t>(i) <= text().size() - mScrollOffset)
	{
		std::string cmpStr = text().substr(mScrollOffset, i);
		int strLen = font().width(cmpStr);
		if(strLen > relativePosition)
		{
			mCursorPosition = i - 1;
			break;
		}

		i++;
	}
}
