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
		TOOLBAR_LAYER_BG_EDIT,
		TOOLBAR_LAYER_BG_DETAIL_EDIT,
		TOOLBAR_LAYER_DETAIL_EDIT,
		TOOLBAR_LAYER_FOREGROUND_EDIT,
		TOOLBAR_LAYER_COLLISION_EDIT,
		TOOLBAR_LAYER_BG_TOGGLE,
		TOOLBAR_LAYER_BG_DETAIL_TOGGLE,
		TOOLBAR_LAYER_DETAIL_TOGGLE,
		TOOLBAR_LAYER_FOREGROUND_TOGGLE,
		TOOLBAR_LAYER_COLLISION_TOGGLE,
		TOOLBAR_MINIMAP_TOGGLE,
		TOOLBAR_TILE_PALETTE_TOGGLE,
		TOOLBAR_TOOL_PENCIL,
		TOOLBAR_TOOL_FILL,
		TOOLBAR_TOOL_ERASER
	};

	typedef NAS2D::Signals::Signal1<ToolBarAction> ToolBarEvent;

public:
	ToolBar();
	~ToolBar();

	void update();

	const std::string map_name() const { return txtMapName.text(); }
	void map_name(const std::string& name) { txtMapName.text(name); }

	ToolBarEvent& toolbar_event() { return mToolbarEvent; }

	bool pencil() const { return btnPencil.toggled(); }
	bool flood() const { return btnFill.toggled(); }
	bool flood_contiguous() const { return btnFillContiguous.toggled(); }
	bool erase() const { return btnErase.toggled(); }

	bool show_bg() const { return btnLayerBaseToggle.toggled(); }
	bool show_bg_detail() const { return btnLayerBaseDetailToggle.toggled(); }
	bool show_detail() const { return btnLayerDetailToggle.toggled(); }
	bool show_foreground() const { return btnLayerForegroundToggle.toggled(); }
	bool show_collision() const { return btnLayerCollisionToggle.toggled(); }

	bool show_minimap() const { return btnMiniMapToggle.toggled(); }
	bool show_tilepalette() const { return btnTilePaletteToggle.toggled(); }

	const Pattern& brush() const { return mBrush; }

	const Rectangle_2d flood_tool_extended_area() const { return mFloodFillExtendedArea; }

private:

	void initUi();

	void hookEvents();

	void onKeyDown(NAS2D::EventHandler::KeyCode code, NAS2D::EventHandler::KeyModifier mod, bool repeat);

	void onMouseWheel(int x, int y);

private:

	void resetTools();

	// UI Event Handlers
	void btnSave_Clicked();
	void btnPencil_Clicked();
	void btnFill_Clicked();
	void btnErase_Clicked();

	void btnLayerBase_Clicked();
	void btnLayerBaseDetail_Clicked();
	void btnLayerDetail_Clicked();
	void btnLayerForeground_Clicked();
	void btnLayerCollision_Clicked();

	void btnSpinnerUp_Clicked();
	void btnSpinnerDown_Clicked();

	void btnLayerBaseToggle_Clicked();
	void btnLayerBaseDetailToggle_Clicked();
	void btnLayerDetailToggle_Clicked();
	void btnLayerForegroundToggle_Clicked();
	void btnLayerCollisionToggle_Clicked();

	void btnMiniMapToggle_Clicked();
	void btnTilePaletteToggle_Clicked();

private:

	Font		mFont;

	Image		mToggle;

	Pattern		mBrush;

	// PRIMITIVES
	Rectangle_2d	mFloodFillExtendedArea;

	// UI ELEMENTS
	TextField	txtMapName;

	Button		btnSave;

	Button		btnPencil;
	Button		btnFill;
	Button		btnFillContiguous;
	Button		btnErase;

	Button		btnLayerBase;
	Button		btnLayerBaseDetail;
	Button		btnLayerDetail;
	Button		btnLayerForeground;
	Button		btnLayerCollision;

	Button		btnSpinnerUp;
	Button		btnSpinnerDown;

	Button		btnLayerBaseToggle;
	Button		btnLayerBaseDetailToggle;
	Button		btnLayerDetailToggle;
	Button		btnLayerForegroundToggle;
	Button		btnLayerCollisionToggle;

	Button		btnMiniMapToggle;
	Button		btnTilePaletteToggle;

	ToolBarEvent	mToolbarEvent;
};