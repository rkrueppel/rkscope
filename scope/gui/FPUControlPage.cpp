#include "stdafx.h"
#include "FPUControlPage.h"

namespace scope {
	namespace gui {

CFPUControlPage::CFPUControlPage(const uint32_t& _area, std::vector<std::unique_ptr<parameters::BaseArea>>& _allareas, FPUButtons& _fpubuttons, const double& _masterfovsizex, const double& _masterfovsizey)
	: areaparams(_allareas[_area].get())
	, strtitle(L"")
	, xpos_edit(areaparams->fpuxystage.xpos, true)
	, ypos_edit(areaparams->fpuxystage.ypos, true)
	, etlcalibrationfile_edit(areaparams->fpuzstage.calibrationfile, true)
	, setxyzero_button(_fpubuttons.setzero)
	, fpustageinfos_edit(areaparams->fpuxystage.stageinfo, true)
	, diagram(_area, _allareas, _masterfovsizex, _masterfovsizey) {
	std::wstringstream stream;
	stream << L"Area " << _area+1;
	strtitle = stream.str();
	SetTitle(strtitle.c_str());			// Be careful, do not assign a local variable (since SetTitle takes only the pointer, which will become invalid for a local variable)
}

BOOL CFPUControlPage::OnInitDialog(CWindow wndFocus, LPARAM lInitParam) {
	xpos_edit.AttachToDlgItem(GetDlgItem(IDC_POSX_EDIT));
	ypos_edit.AttachToDlgItem(GetDlgItem(IDC_POSY_EDIT));
	etlcalibrationfile_edit.AttachToDlgItem(GetDlgItem(IDC_ETLCALIBRATIONFILE_EDIT));
	setxyzero_button.AttachToDlgItem(GetDlgItem(IDC_SETXYZERO_BUTTON));
	fpustageinfos_edit.AttachToDlgItem(GetDlgItem(IDC_STANDAINFOS_EDIT));
	
	diagram.AttachToDlgItem(GetDlgItem(IDC_FPUDIAGRAM));

	SetMsgHandled(false);
	return 0;
}

LRESULT CFPUControlPage::OnCalibrationFileButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	COMDLG_FILTERSPEC fileTypes[] = {{ L"ETL calibration file", L"*.txt" }};
	CShellFileSaveDialog dlg(NULL, FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST, L"txt", fileTypes, 1);
	dlg.GetPtr()->SetTitle(L"Select Fast Z calibration file");
	IShellItem *psiFolder;
	SHCreateItemFromParsingName(L"C:\\Temp\\", NULL, IID_PPV_ARGS(&psiFolder));
	dlg.GetPtr()->SetFolder(psiFolder);

	if ( IDOK == dlg.DoModal(::GetDesktopWindow()) ) {			// Crashes without GetDesktopWindow (see here: http://comments.gmane.org/gmane.comp.windows.wtl/16780), CMainDlgFrame is probably not a top level window?!
		CString filepath;
		dlg.GetFilePath(filepath);
		std::wstring strfilepath(filepath.GetString());
		DBOUT(L"Fast Z calibration file loaded from " << strfilepath);
		areaparams->fpuzstage.LoadCalibration(strfilepath);
	}
	return 0;
}

}
}