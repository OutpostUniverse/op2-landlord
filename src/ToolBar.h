#pragma once

#include "NAS2D/NAS2D.h"

#include "Button.h"
#include "TextField.h"

using namespace NAS2D;


class ToolBar
{
public:
	enum ToolBarAction
	{
		TOOLBAR_SAVE,
		TOOLBAR_PENCIL,
		TOOLBAR_FILL,
		TOOLBAR_ERASE,
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
		TOOLBAR_TILE_PALETTE_TOGGLE
	};

	typedef Gallant::Signal2<ToolBarAction, bool> ToolBarEvent;

public:
	ToolBar();
	~ToolBar();

	void update();

	const std::string map_name() const { return txtMapName.text(); }
	void map_name(const std::string& name) { txtMapName.text(name); }

	ToolBarEvent& toolbar_event() { return mToolbarEvent; }

private:

	void initUi();

	void hookEvents();

	void onKeyDown(KeyCode code, KeyModifier mod, bool repeat);

private:

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

private:

	Font		mFont;

	TextField	txtMapName;

	Button		btnSave;

	Button		btnPencil;
	Button		btnFill;
	Button		btnErase;

	Button		btnLayerBase;
	Button		btnLayerBaseDetail;
	Button		btnLayerDetail;
	Button		btnLayerForeground;
	Button		btnLayerCollision;

	Button		btnLayerBaseToggle;
	Button		btnLayerBaseDetailToggle;
	Button		btnLayerDetailToggle;
	Button		btnLayerForegroundToggle;
	Button		btnLayerCollisionToggle;

	ToolBarEvent	mToolbarEvent;
};