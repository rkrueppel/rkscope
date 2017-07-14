#include "stdafx.h"
#include "ScopeColorComboCtrl.h"
#include "helpers\lut.h"
#include "resource.h"

namespace scope {
	namespace gui {

CImageList CScopeColorComboCtrl::imagelist;

CScopeColorComboCtrl::CScopeColorComboCtrl() {
	// Create image list (only once)
	if ( imagelist.IsNull() )
		imagelist.Create(MAKEINTRESOURCE(IDB_COLORCOMBO), 16, 16, ILC_COLOR24);

	// possible resource leak -> imagelist is never destroyed... (I do not care, at program exit everything gets flushed anyway)
}

void CScopeColorComboCtrl::InitializeColors() {
	SetImageList(imagelist);

	COMBOBOXEXITEM cbi;

	for ( uint32_t i = 0 ; i < gColorStrings.size() ; i++ ) {
		cbi.iItem = i;
		cbi.pszText = (LPWSTR)(LPCWSTR)gColorStrings[i];
		cbi.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
		cbi.cchTextMax = gColorStrings[i].GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;
		InsertItem(&cbi);
	}
	
}


}

}