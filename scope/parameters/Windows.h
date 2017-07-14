#pragma once

#include "Base.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"

namespace scope {
	namespace parameters {

/** Parameters for all frames/windows on screen
* @ingroup ScopeParameters */
class Window
	: public Base {

public:
	/** Type of window, e.g. CChannelFrame or CHistogramFrame */
	ScopeString type;

	/** For which area that window is */
	ScopeNumber<uint32_t> area;

	/** top coordinate */
	ScopeNumber<uint32_t> top;

	/** left coordinate */
	ScopeNumber<uint32_t> left;

	/** bottom coordinate */
	ScopeNumber<uint32_t> bottom;

	/** right coordinate */
	ScopeNumber<uint32_t> right;

	Window(void);

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override { }
};

/** Parameters for all frames/windows on screen
* @ingroup ScopeParameters */
class WindowCollection
	: public Base {

public:
	/** Vector with Window */
	std::vector<Window> collection;

	WindowCollection(void);

	/** Add a window to the collection */
	void AddWindow(const std::wstring& _type, const uint32_t& _area, HWND _hwnd);

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override { }
};

}
}