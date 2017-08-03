#pragma once

#include "ScopeDefines.h"
#include "helpers/ScopeDatatypes.h"
#include "helpers/ScopeNumber.h"
#include "helpers/ScopeString.h"
#include "helpers/helpers.h"
#include "Base.h"

namespace scope {

/** all parameters live in this namespace */
namespace parameters {

using boost::property_tree::wptree;

/** Parameters for storage
* @ingroup ScopeParameters */
class Storage
	: public Base {
public:
	Storage();

	/** folder to Save into */
	ScopeString folder;
	
	/** basename for image files */
	ScopeString basename;
	
	/** autosave all acquisitions (except live scan) */
	ScopeNumber<bool> autosave;
	
	/** autosave live scan acquisitions */
	ScopeNumber<bool> savelive;
	
	/** Save ScopeParameters in xml file */
	ScopeNumber<bool> saveparameters;

	/** write tiff tags for ImageJ etc. */
	ScopeNumber<bool> usetifftags;

	/** write compressed TIFF */
	ScopeNumber<bool> compresstiff;

	void Load(const wptree& pt) override;
	void Save(wptree& pt) const override;
	void SetReadOnlyWhileScanning(const RunState& _runstate) override;
};

}

}