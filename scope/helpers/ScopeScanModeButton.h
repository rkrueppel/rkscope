#pragma once
#include "ScopeButton.h"
#include "helpers/ScopeDatatypes.h"

namespace scope {

/** Button type for scan mode switching */
class ScopeScanModeButton : public ScopeButton {

public:
	/** The type of scannervector underlying the button */
	const ScannerVectorType mode;

	ScopeScanModeButton(const ScannerVectorTypeHelper::Mode& _mode);

};

}