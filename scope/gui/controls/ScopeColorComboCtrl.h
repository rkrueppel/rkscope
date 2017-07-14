#pragma once

namespace scope {
	namespace gui {

/** Simple class derivec from CComboBoxEx, added only method InitializeColors wich fills combo with color icons and text
* from lut.h. To add to a toolbar use my CreateToolbarComboBoxEx added to ToolBarHelper */
class CScopeColorComboCtrl
	: public CComboBoxEx {

protected:
	/** The image list with the color bitmap */
	static CImageList imagelist;

public:
	CScopeColorComboCtrl();

	/** fill items */
	void InitializeColors();
};

}

}