#include "stdafx.h"
#include "ScopeLEDCtrl.h"
#include "resource.h"
#include "helpers/helpers.h"

namespace scope {

namespace gui {

CImageList CScopeLEDCtrl::imagegreen;
CImageList CScopeLEDCtrl::imagered;
CImageList CScopeLEDCtrl::imagewarning;

CScopeLEDCtrl::CScopeLEDCtrl(ScopeNumber<bool>* _scopebool, const uint32_t& _style)
	: scopebool(_scopebool)
	, style(_style) {
	// Create imagelists (only once)
	if ( imagegreen.IsNull() ) {
		CIcon icon;
		icon.LoadIconWithScaleDown(IDI_LED_GREEN, 18, 18);
		imagegreen.Create(18, 18, ILC_COLOR32, 0, 2);
		imagegreen.AddIcon(icon);
	}
	if ( imagered.IsNull() ) {
		CIcon icon;
		icon.LoadIconWithScaleDown(IDI_LED_RED, 18, 18);
		imagered.Create(18, 18, ILC_COLOR32, 0, 2);
		imagered.AddIcon(icon);
	}
	if ( imagewarning.IsNull() ) {
		CIcon icon;
		icon.LoadIconWithScaleDown(IDI_LED_WARNING, 18, 18);
		imagewarning.Create(18, 18, ILC_COLOR32, 0, 2);
		imagewarning.AddIcon(icon);
	}
	valueconnection = scopebool->ConnectGUI(std::bind(&CScopeLEDCtrl::UpdateControl, this));
}

CScopeLEDCtrl::~CScopeLEDCtrl() {
	valueconnection.disconnect();
}

void CScopeLEDCtrl::UpdateControl() {
	if ( scopebool->Value() )
		SetTrue();
	else
		SetFalse();
}

bool CScopeLEDCtrl::AttachToDlgItem(HWND hWnd) {
	if ( SubclassWindow(hWnd) ) {
		UpdateControl();
		return true;
	}
	return false;
}

void CScopeLEDCtrl::DoPaint(CDCHandle dc) {
	RECT rc;
	GetClientRect(&rc);
	dc.FillRect(&rc, (HBRUSH)(COLOR_BTNFACE));
	// call inherited DoPaint()
	CBitmapButtonImpl<CScopeLEDCtrl>::DoPaint(dc);
}

void CScopeLEDCtrl::SetTrue() {
	if ( style == SCOPELED_OKONTRUE )
		SetImageList(imagegreen);
	if ( style == SCOPELED_WARNINGONTRUE )
		SetImageList(imagewarning);
	SetImages(0);
	// Force drawing update
	if ( IsWindow() )
		Invalidate();
}

void CScopeLEDCtrl::SetFalse() {
	if ( style == SCOPELED_OKONTRUE )
		SetImageList(imagered);
	if ( style == SCOPELED_WARNINGONTRUE )
		SetImageList(imagegreen);
	SetImages(0);
	// Force drawing update
	if ( IsWindow() )
		Invalidate();
}

}

}