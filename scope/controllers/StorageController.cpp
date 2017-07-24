#include "stdafx.h"
#include "StorageController.h"

namespace scope {

StorageController::StorageController(const uint32_t& _nactives, const parameters::Scope& _parameters, SynchronizedQueue<ScopeMessage<SCOPE_MULTIIMAGEPTR_T>>* const _iqueue)
	: BaseController(_nactives, _parameters)
	, input_queue(_iqueue)
	, runcounter(0)
	, filenames(_nactives)
	, encoders(_nactives) {
}

StorageController::~StorageController() {
	StopAll();
	WaitForAll(-1);
}

void StorageController::StopOne(const uint32_t& _a) {
	BaseController::StopOne(_a);
	ScopeMessage<SCOPE_MULTIIMAGEPTR_T> stopmsg(ScopeMessageTag::abort, nullptr);
	input_queue->Enqueue(stopmsg);
}

ControllerReturnStatus StorageController::Run(StopCondition* const sc, const uint32_t& _area) {
	DBOUT(L"StorageController::Run beginning\n");
	ControllerReturnStatus returnstatus(ControllerReturnStatus::none);
	uint32_t framearea = 0;
	std::wstring foldername(L"");
	std::vector<ScopeMultiImagePtr> current_frames(nactives);
	const DaqMode requested_mode = parameters.requested_mode();
	std::vector<uint32_t> requested_frames(nactives);
	uint32_t a = 0;
	std::generate(std::begin(requested_frames), std::end(requested_frames),
		[&]() { return parameters.areas[a++]->daq.requested_frames(); } );
	// Checks if we should really do saving
	bool dosave(parameters.storage.autosave()
		&& ( (requested_mode == DaqModeHelper::nframes)
			|| ((requested_mode == DaqModeHelper::continuous) && parameters.storage.savelive()) ) );

	// If saving desired create folder and encoders
	try {
		if ( dosave ) {
			foldername = CreateFolder();
			// Save ScopeParameters in xml if desired
			if ( parameters.storage.saveparameters() )
				parameters.Save(foldername + L"parameters.xml");
			++runcounter;
		}
	} catch (...) {
		ScopeExceptionHandler(__FUNCTION__, true, true);
		dosave = false;
	}
	
	bool reqEqual, framecountEqual;
	std::vector<uint32_t> totalframecount(nactives, 0);
	InitializeEncoders(dosave, foldername);

	// dequeue and save loop
	while ( !sc->IsSet() ) {
		// Dequeue
		ScopeMessage<SCOPE_MULTIIMAGEPTR_T> msg(input_queue->Dequeue());

		// If message has abort tag, break from while loop
		if ( msg.tag == ScopeMessageTag::abort ) {
			returnstatus = ControllerReturnStatus::stopped;
			break;
		}
		DBOUT(L"StorageController::impl::Run dequeued\n");

		framearea = msg.cargo->Area();
		current_frames[framearea] = msg.cargo;

		// otherwise something is seriously wrong:
		assert(parameters.areas[framearea]->daq.inputs->channels() == current_frames[framearea]->Channels());

		// Create new frame on disk (Frames are only actually saved if encoder was created with dosave=true)
		encoders[framearea]->NewFrame();
		// and write into it
		encoders[framearea]->WriteFrame(current_frames[framearea]);

		reqEqual = false;
		framecountEqual = false;
		const uint32_t maxframes = 3000000000 / ( current_frames[framearea]->Linewidth()*current_frames[framearea]->Lines()*16/8 );
		// Preparation for the conditions below
		for ( uint32_t i = 0; i < nactives; i++ ){
			reqEqual = reqEqual || ( requested_frames.at(i) == encoders.at(i)->Framecount() + totalframecount.at(i) );
			framecountEqual = framecountEqual || ( encoders.at(i)->Framecount() == maxframes );
		}

		// Check if in nframes mode if we have already stored all requested frames for all areas
		if ( (requested_mode == DaqModeHelper::nframes) && reqEqual ) {
			// If yes we want to stop
			sc->Set(true);
			returnstatus = ControllerReturnStatus::finished;
			DBOUT(L"StorageController::Impl::Run - all requested frames from all areas saved\n");
		}

		// Check if the old file has to be closed and a new one opened
		if ( framecountEqual ) {
			for ( uint32_t i = 0; i < nactives; i++ ) {
				totalframecount.at(i)+= encoders.at(i)->Framecount();

				// This calls encoders destructors, thus all files are closed and can be TIFF-fixed
				for ( auto& e : encoders )
					e.reset(nullptr);

				// Fix the tiff tags if wanted
				if ( dosave && parameters.storage.usetifftags() )
					FixTIFFTags();

				runcounter++;
				InitializeEncoders(dosave, foldername);
			}
		}
	}

	// This calls encoders destructors, thus all files are closed and can be TIFF-fixed
	for ( auto& e : encoders )
		e.reset(nullptr);

	// Fix the tiff tags if wanted
	if ( dosave && parameters.storage.usetifftags() )
		FixTIFFTags();

	if ( sc->IsSet() )
		returnstatus = (ControllerReturnStatus)(returnstatus || ControllerReturnStatus::stopped);

	ATLTRACE(L"StorageController::Run end\n");
	return returnstatus;
}

std::wstring StorageController::CreateFolder() {
	// Make suffic depending on run state
	std::wstring runmode(L"");
	switch ( parameters.run_state().t ) {
	case RunStateHelper::RunningContinuous:
		runmode = L"_Live";
		break;
	case RunStateHelper::RunningSingle:
		runmode = L"_Single";
		break;
	case RunStateHelper::RunningStack:
		runmode = L"_Stack";
		break;
	case RunStateHelper::RunningTimeseries:
		runmode = L"_Timeseries";
		break;
	}

	std::wstringstream foldername;
	foldername << parameters.storage.folder() << parameters.date() << L"\\" << parameters.time() << runmode << L"\\";

	std::wstring msg = L"Saving into " + foldername.str();
	scope_logger.Log(msg, log_info);

	// Create directory
	if ( SHCreateDirectoryEx(NULL, foldername.str().c_str(), NULL) == ERROR_SUCCESS )
		return foldername.str();
	else
		throw ScopeException(__FUNCTION__);

	return L"";
}

void StorageController::InitializeEncoders(const bool& _dosave, const std::wstring& _foldername) {
	for ( uint32_t a = 0 ; a < nactives; a++ ) {
		// Make a new multi image encoder for that area
		encoders[a] = std::unique_ptr<ScopeMultiImageEncoder>(new ScopeMultiImageEncoder(_dosave, parameters.areas[a]->daq.inputs->channels(), parameters.storage.compresstiff()));
		filenames[a].resize(parameters.areas[a]->daq.inputs->channels());
		// Construct the filenames for all channels
		for ( uint32_t c = 0 ; c < parameters.areas[a]->daq.inputs->channels() ; c++ ) {
			std::wstringstream stream;
			stream << _foldername << parameters.storage.basename() << L"_A" << a << L"_Ch" << c << L"_ " << std::setfill(L'0') << std::setw(4) << runcounter << L".tif";
			filenames[a][c] = stream.str();
		}
		// Give the filenames to the encoder
		encoders[a]->Initialize(filenames[a]);
	}
}

void StorageController::FixTIFFTags() {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// Prepare process
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	WCHAR buff[255];
	GetCurrentDirectory(255, buff);

	// Build command line string for exiftool.exe
	std::wstringstream cmdbase;
	cmdbase << L"-delete_original -overwrite_original_in_place -ResolutionUnit=None -Software=\"Proudly recorded with Scope\" ";
	switch ( parameters.run_state().t ) {
		case RunStateHelper::RunningStack:
			cmdbase << L"-ImageDescription=\"ImageJ=1.47m\nunit=um\nspacing=" << parameters.stack.spacing() << L"\" ";
			break;
		case RunStateHelper::RunningTimeseries:
			break;
		default:
			cmdbase << L"-ImageDescription=\"ImageJ=1.47m\nunit=um\" ";
	}

	// Go through all areas (run exiftool.exe once per area)
	for ( uint32_t a = 0 ; a < nactives; a++ ) {
		std::wstringstream cmd;
		cmd << cmdbase.str();
		// Add timeseries stuff
		if ( parameters.run_state() == RunStateHelper::RunningTimeseries ) {
			cmd << L"-ImageDescription=\"ImageJ=1.47m\nunit=um\nfinterval=" << parameters.timeseries.totaltimes[a]()/parameters.timeseries.frames[a]();
			cmd << L"\nframes=" << parameters.timeseries.frames[a]() << L"\" ";
		}

		// Add µm resolution
		cmd << L"-XResolution=" << 1/parameters.areas[a]->micronperpixelx() << L" -YResolution=" << 1/parameters.areas[a]->micronperpixely() << L" ";

		// Add the filenames
		for ( uint32_t c = 0 ; c < parameters.areas[a]->daq.inputs->channels() ; c++ )
			cmd << L"\"" << filenames[a][c] << L"\" ";

		// Run exiftool.exe
		DBOUT(L"Cmd: " << cmd.str());
		std::wstring cmdstr(cmd.str());
		std::vector<wchar_t> wincmd(cmdstr.begin(), cmdstr.end());
		if ( 0 == CreateProcess(L"tools/exiftool.exe", wincmd.data(), NULL, NULL, FALSE, BELOW_NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi) ) {
			std::wstringstream errstr;
			errstr << L"exiftool.exe did not start with error " << GetLastError();
			scope_logger.Log(errstr.str(), log_error);
			return;
		}

		// Wait 1 second for exiftool.exe to finish
		DWORD procret = WaitForSingleObject(pi.hProcess, 1000);
		if ( WAIT_OBJECT_0 != procret ) {
			std::wstringstream errstr;
			errstr << L"Waiting for exiftool failed with error " << procret;
			scope_logger.Log(errstr.str(), log_error);
			return;
		}

		// Close process handle
		CloseHandle(pi.hProcess);
		if (0 == CloseHandle(pi.hThread) ) {
			std::wstringstream errstr;
			errstr << L"exiftool.exe did not finish correctly, error " << GetLastError();
			scope_logger.Log(errstr.str(), log_error);
			return;
		}
	}

}


}