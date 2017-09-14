#include "stdafx.h"
#include "ScopeScanModeButton.h"

namespace scope {

	ScopeScanModeButton::ScopeScanModeButton(const ScannerVectorTypeHelper::Mode& _mode)
		: ScopeButton(ScannerVectorTypeHelper::NameOf(_mode))
		, mode(_mode)
	{
	}


}
