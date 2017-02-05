#pragma once

#include "NAS2D/NAS2D.h"

#include "Button.h"
#include "TextField.h"

using namespace NAS2D;


class ToolBar
{
public:
	ToolBar();
	~ToolBar();

	void update();

	const std::string map_name() const { return txtMapName.text(); }
	void map_name(const std::string& name) { txtMapName.text(name); }

private:

	void initUi();

	void hookEvents();

	void onKeyDown(KeyCode code, KeyModifier mod, bool repeat);

private:

	// UI Event Handlers
	void btnLayerBase_Clicked();
	void btnLayerBaseDetail_Clicked();
	void btnLayerDetail_Clicked();
	void btnLayerForeground_Clicked();
	void btnLayerCollision_Clicked();

private:

	Font		mFont;

	TextField	txtMapName;

	Button		btnSave;

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

};