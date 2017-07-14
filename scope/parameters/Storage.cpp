#include "stdafx.h"
#include "parameters/Storage.h"
#include "helpers/ScopeException.h"

namespace scope {

namespace parameters {

// Save some typing here...
using namespace boost::property_tree;

Storage::Storage()
	: folder(L"C:\\ScopeData\\", L"Folder")
	, basename(L"test", L"Basename")
	, autosave(true, false, true, L"Autosave")
	, savelive(false, false, true, L"SaveLive")
	, saveparameters(true, false, true, L"SaveParameters")
	, usetifftags(true, false, true, L"UseTIFFTags")
	, compresstiff(true, false, true, L"CompressTIFF") {
}

void Storage::Load(const wptree& pt) {
	autosave.SetFromPropertyTree(pt);
	savelive.SetFromPropertyTree(pt);
	basename.SetFromPropertyTree(pt);
	folder.SetFromPropertyTree(pt);
	saveparameters.SetFromPropertyTree(pt);
	usetifftags.SetFromPropertyTree(pt);
	compresstiff.SetFromPropertyTree(pt);
}

void Storage::Save(wptree& pt) const {
	autosave.AddToPropertyTree(pt);
	savelive.AddToPropertyTree(pt);
	basename.AddToPropertyTree(pt);
	folder.AddToPropertyTree(pt);
	saveparameters.AddToPropertyTree(pt);
	usetifftags.AddToPropertyTree(pt);
	compresstiff.AddToPropertyTree(pt);
}

void Storage::SetReadOnlyWhileScanning(const RunState& _runstate) {
	const bool enabler = (_runstate.t==RunStateHelper::Mode::Stopped)?true:false;
	autosave.SetRWState(enabler);
	savelive.SetRWState(enabler);
	basename.SetRWState(enabler);
	folder.SetRWState(enabler);
	saveparameters.SetRWState(enabler);
	usetifftags.SetRWState(enabler);
	compresstiff.SetRWState(enabler);
}

}

}