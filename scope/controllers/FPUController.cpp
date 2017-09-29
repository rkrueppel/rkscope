#include "StdAfx.h"
#include "FPUController.h"
#include "helpers/ScopeDatatypes.h"

namespace scope {

FPUController::FPUController(const uint32_t& _nareas, std::vector<parameters::Area>& _guiareaparamsvec, std::vector<FPUButtons>& _fpubuttonsvec)
	: nareas(_nareas)
	, guiareaparamsvec(_guiareaparamsvec)
	, stepsizes(_nareas, 0.0)
{
	//theXYStages.reserve(nareas);
	for ( uint32_t a = 0 ; a < _nareas ; a++ ) {
		theXYStages.emplace_back(std::make_unique<SCOPE_FPUXYCONTROL_T>(guiareaparamsvec[a].fpuxystage));
		
		stepsizes[a] = guiareaparamsvec[a].fpuxystage.buttonstepsize();
		
		guiareaparamsvec[a].fpuxystage.xpos.ConnectOther(std::bind(&FPUController::MoveAbsolute, this, a));
		guiareaparamsvec[a].fpuxystage.ypos.ConnectOther(std::bind(&FPUController::MoveAbsolute, this, a));

		_fpubuttonsvec[a].left.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(left)));
		_fpubuttonsvec[a].right.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(right)));
		_fpubuttonsvec[a].up.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(up)));
		_fpubuttonsvec[a].down.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(down)));
		_fpubuttonsvec[a].setzero.Connect(std::bind(&FPUController::SetXYZero, this, a));
	}
}

void FPUController::Initialize() {
	// This is called from TheScope after loading the initial parameter set
	// Only then should the stages be initialized (who knows if the default constructed parameters do harm to the stage)
	for ( auto& xy : theXYStages )
		xy->Initialize();
}

void FPUController::MoveAbsolute(const uint32_t& _area) {
	DBOUT(L"FPUController::MoveAbsolute");
	const double newx = guiareaparamsvec[_area].fpuxystage.xpos();
	const double newy = guiareaparamsvec[_area].fpuxystage.ypos();
	theXYStages[_area]->MoveAbsolute(newx, newy);
}

void FPUController::MoveRelative(const uint32_t& _area, const FPUMoveDirection& _dir) {
	DBOUT(L"FPUController::MoveRelative direction " << _dir);
	switch (_dir) {
		case left:
			theXYStages[_area]->MoveRelative(-stepsizes[_area], 0);
			break;
		case right:
			theXYStages[_area]->MoveRelative(stepsizes[_area], 0);
			break;
		case up:
			theXYStages[_area]->MoveRelative(0, -stepsizes[_area]);
			break;
		case down:
			theXYStages[_area]->MoveRelative(0, stepsizes[_area]);
	}
}

void FPUController::SetXYZero(const uint32_t& _area) {
	int ret = ::MessageBox(NULL, L"Do you really want to set the current FPU stage position as zero?", L"Are you sure?", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2 | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
	if ( ret == IDYES )
		theXYStages[_area]->SetZero();
}

}