#pragma once

#include "NAS2D/NAS2D.h"

#include "Button.h"
#include "TextField.h"

#include "Pattern.h"

using namespace NAS2D;


class ToolBar
{
public:
	enum ToolBarAction
	{
		TOOLBAR_SAVE,
		TOOLBAR_MINIMAP_TOGGLE,
		TOOLBAR_TILE_GROUPS_TOGGLE,
		TOOLBAR_TILE_PALETTE_TOGGLE,
		TOOLBAR_TOOL_PENCIL,
		TOOLBAR_TOOL_FILL,
		TOOLBAR_TOOL_ERASER,
		TOOLBAR_QUIT
	};

	typedef NAS2D::Signals::Signal1<ToolBarAction> ToolBarEvent;

public:
	ToolBar();
	~ToolBar();

	void update();

	ToolBarEvent& toolbar_event() { return mToolbarEvent; }

	bool pencil() const { return btnPencil.toggled(); }
	bool flood() const { return btnFill.toggled(); }
	bool flood_contiguous() const { return btnFillContiguous.toggled(); }
	bool erase() const { return btnErase.toggled(); }

	bool show_collision_mask() const { return btnLayerCollision.toggled(); }

	bool show_minimap() const { return btnMiniMapToggle.toggled(); }
	bool show_tilepalette() const { return btnTilePaletteToggle.toggled(); }
	bool show_tilegroups() const { return btnTileGroupsToggle.toggled(); }

	const Pattern& brush() const { return mBrush; }

	const Rectangle_2d flood_tool_extended_area() const { return mFloodFillExtendedArea; }

	int height();

private:
	void initUi();

	void hookEvents();

	void onKeyDown(NAS2D::EventHandler::KeyCode code, NAS2D::EventHandler::KeyModifier mod, bool repeat);
	void onMouseWheel(int x, int y);
	void onWindowResized(int width, int height);

private:
	void resetTools();

	// UI Event Handlers
	void btnSave_Clicked();
	void btnPencil_Clicked();
	void btnFill_Clicked();
	void btnErase_Clicked();

	void btnSpinnerUp_Clicked();
	void btnSpinnerDown_Clicked();

	void btnMiniMapToggle_Clicked();
	void btnTilePaletteToggle_Clicked();
	void btnTileGroupsToggle_Clicked();

	void btnExit_Clicked();

private:
	Font			mFont;

	Image			mToggle;

	Pattern			mBrush;

	// PRIMITIVES
	Rectangle_2d	mFloodFillExtendedArea;

	// UI ELEMENTS
	Button			btnSave;

	Button			btnPencil;
	Button			btnFill;
	Button			btnFillContiguous;
	Button			btnErase;

	Button			btnLayerCollision;

	Button			btnSpinnerUp;
	Button			btnSpinnerDown;

	Button			btnMiniMapToggle;
	Button			btnTilePaletteToggle;
	Button			btnTileGroupsToggle;

	Button			btnExit;

	ToolBarEvent	mToolbarEvent;
};