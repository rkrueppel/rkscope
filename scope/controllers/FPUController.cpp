#include "StdAfx.h"
#include "FPUController.h"
#include "ScopeDatatypes.h"

namespace scope {

FPUController::FPUController() {
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ ) {
		theXYStages[a] = std::unique_ptr<SCOPE_FPUXYCONTROL_T>(new SCOPE_FPUXYCONTROL_T());
		stepsizes[a] = scope_controller.GuiParameters.areas[a]->fpuxystage.buttonstepsize();
		scope_controller.GuiParameters.areas[a]->fpuxystage.xpos.ConnectOther(std::bind(&FPUController::MoveAbsolute, this, a));
		scope_controller.GuiParameters.areas[a]->fpuxystage.ypos.ConnectOther(std::bind(&FPUController::MoveAbsolute, this, a));
		scope_controller.FPU[a].LeftButton.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(left)));
		scope_controller.FPU[a].RightButton.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(right)));
		scope_controller.FPU[a].UpButton.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(up)));
		scope_controller.FPU[a].DownButton.Connect(std::bind(&FPUController::MoveRelative, this, a, FPUMoveDirection(down)));
		scope_controller.FPU[a].SetZeroButton.Connect(std::bind(&FPUController::SetXYZero, this, a));
	}
}

void FPUController::Initialize(const parameters::Scope& _params) {
	for ( uint32_t a = 0 ; a < SCOPE_NAREAS ; a++ )
		theXYStages[a]->Initialize(_params.areas[a]->fpuxystage);	
}

void FPUController::MoveAbsolute(const uint32_t& _area) {
	DBOUT(L"FPUController::MoveAbsolute");
	const double newx = scope_controller.GuiParameters.areas[_area]->fpuxystage.xpos();
	const double newy = scope_controller.GuiParameters.areas[_area]->fpuxystage.ypos();
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