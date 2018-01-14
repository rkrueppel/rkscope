#include "stdafx.h"
#include "DAQmxTask.h"
#include "helpers/ScopeException.h"
#include "helpers/Helpers.h"

namespace DAQmx {

bool CheckError(const int32& error) {
	if ( DAQmxFailed(error) ) {
		char perrorstring[512];
		DAQmxGetErrorString(error, perrorstring, 512);
		DAQmxGetExtendedErrorInfo(perrorstring, 512);
		DBOUT(L"DAQmx exception: " << perrorstring)
		if ( DAQMX_THROW_EXCEPTION )
			throw scope::ScopeException(perrorstring);

		return true;
	}
	return false;
}

double PredictSampleRate(const double& _desiredrate, const uint32_t& _nochannels, const double& _refclockrate, const int32& _mode) {
	const double divider = std::floor(_refclockrate / (_desiredrate / static_cast<double>(_nochannels)));
	const double rate1 = (_refclockrate / divider) * static_cast<double>(_nochannels);
	const double rate2 = (_refclockrate / (divider+1)) * static_cast<double>(_nochannels);
	switch (_mode) {
		case DAQmx_Val_AI:
			return rate1;
		case DAQmx_Val_AO:
			return (abs(rate1-_desiredrate) < abs(rate2-_desiredrate))?rate1:rate2;
		default:
			return -1;
	}
}

double CoerceSampleRates(const double& _desiredrate, const uint32_t& _nochannelsin, const uint32_t& _nochannelsout, const double& _refclockrate) {
	double ratein = PredictSampleRate(_desiredrate, _nochannelsin, _refclockrate, DAQmx_Val_AI);
	double rateout = PredictSampleRate(_desiredrate, _nochannelsout, _refclockrate, DAQmx_Val_AO);
	if ( ratein == rateout )
		return ratein;
	else		// rateout can only be == or < ratein (see PredictSampleRate)
		return rateout;
}

std::wstring ClockString(const scope::DaqTimingHelper::Mode& _timing, const std::wstring& _externalclocksource) {
	switch ( _timing ) {
	case scope::DaqTimingHelper::Mode::OnboardClock:
			return L"OnboardClock";
			break;
	case scope::DaqTimingHelper::Mode::External:
			return _externalclocksource;
			break;
	default:
			return L"";					// For reference clock leave blank!
	}
}

/*
static int32 GetTerminalNameWithDevPrefix(TaskHandle taskHandle, const char terminalName[], char triggerName[]) {
	int32	error=0;
	char	device[256];
	int32	productCategory;
	uInt32	numDevices,i=1;

	CheckError(DAQmxGetTaskNumDevices(taskHandle,&numDevices));
	while( i<=numDevices ) {
		CheckError (DAQmxGetNthTaskDevice(taskHandle,i++,device,256));
		CheckError (DAQmxGetDevProductCategory(device,&productCategory));
		if( productCategory!=DAQmx_Val_CSeriesModule && productCategory!=DAQmx_Val_SCXIModule ) {
			*triggerName++ = '/';
			strcpy(triggerName,device);					// TODO: Make these safe, use static arrays for char[] with fixed size
			strcat(triggerName,"/");
			strcat(triggerName,terminalName);
			break;
		}
	}

	return error;
}*/

CDAQmxTask::CDAQmxTask()
	: task_handle(nullptr)
{}

CDAQmxTask::~CDAQmxTask() {
	if ( task_handle != nullptr )				// be super-safe here, destructor could be called by exception in constructor...
		DAQmxClearTask(task_handle);			// So, also do not call CheckError here, because we could get an exception, call the destructor by stack-unwinding, -> infinite loop
}

void CDAQmxTask::CreateTask(const std::wstring& _name) {
	CW2A char_name(_name.c_str());
	CheckError(DAQmxCreateTask(char_name, &task_handle));
}

bool CDAQmxTask::Created() const {
	return task_handle != nullptr;
}

void CDAQmxTask::ConfigureImplicitTiming(const int32& _mode, const int32& _samplesperchan) {
	if ( (_mode != DAQmx_Val_FiniteSamps) && (_mode !=DAQmx_Val_ContSamps) )
		throw std::exception("Timing mode invalid");

	CheckError(DAQmxCfgImplicitTiming(task_handle,
		DAQmx_Val_ContSamps,
		_samplesperchan));
}

void CDAQmxTask::ConfigureSampleTiming(const std::wstring& _src, const float64& _rate, const int32& _samplesperchan, const int32& _samplingtype, const int32& _actedge) {
	if ( (_samplingtype != DAQmx_Val_FiniteSamps) && (_samplingtype != DAQmx_Val_ContSamps) )
		throw std::exception("Timing mode invalid");

	if ( (_actedge != DAQmx_Val_Rising) && (_actedge != DAQmx_Val_Falling) )
		throw std::exception("Active edge invalid");

	CW2A char_src(_src.c_str());

	CheckError(DAQmxCfgSampClkTiming(task_handle,
		char_src,
		_rate,
		_actedge,
		_samplingtype,
		_samplesperchan));


}

void CDAQmxTask::ConfigureReferenceClock(const std::wstring& _src, const float64& _rate) {
	CheckError(DAQmxSetTimingAttribute(task_handle, DAQmx_RefClk_Rate, _rate));
	CW2A char_src(_src.c_str());
	CheckError(DAQmxSetTimingAttribute(task_handle, DAQmx_RefClk_Src, (LPCSTR)char_src));
}

void CDAQmxTask::ConfigureDigStartTrigger(const std::wstring& _src, const int32& _trigedge) {
	if ( (_trigedge != DAQmx_Val_Rising) && (_trigedge != DAQmx_Val_Falling) )
		throw std::exception("Trigger edge invalid");

	CW2A char_src(_src.c_str());

	CheckError(DAQmxCfgDigEdgeStartTrig(task_handle,
		char_src,
		_trigedge));
}

void CDAQmxTask::Start(void) {
	CheckError(DAQmxStartTask(task_handle));
}

void CDAQmxTask::Stop(void) {
	CheckError(DAQmxStopTask(task_handle));
}

void CDAQmxTask::Clear(void) {
	CheckError(DAQmxClearTask(task_handle));
}

bool CDAQmxTask::IsTaskDone(void) {
	bool32 isdone;
	CheckError(DAQmxIsTaskDone(task_handle, &isdone));
	return isdone!=0;						// safe conversion from int to bool, simple casting causes compiler warning
}

int32_t CDAQmxTask::WaitUntilDone(const float64& _waittime) {
	return DAQmxWaitUntilTaskDone(task_handle, _waittime);
}

void CDAQmxTask::ExportSignal(int32 _signal, const std::wstring& _terminal) {
	CW2A char_terminal(_terminal.c_str());
	DAQmxExportSignal(task_handle, _signal, char_terminal);
}

void CDAQmxTask::SetWriteOffset(int32 posreltofirst) {
	DAQmxSetWriteAttribute(task_handle, DAQmx_Write_RelativeTo, DAQmx_Val_FirstSample);
	DAQmxSetWriteAttribute(task_handle, DAQmx_Write_Offset, posreltofirst);
}

void CDAQmxTask::SetRegeneration(const bool& _regenerate) {
	int32 mode = (_regenerate == true)?DAQmx_Val_AllowRegen:DAQmx_Val_DoNotAllowRegen;
	DAQmxSetWriteAttribute(task_handle, DAQmx_Write_RegenMode, mode);
}

void CDAQmxAnalogOutTask::CreateAOVoltageChannel(const std::wstring& devices,
	const std::wstring& channelname,
	float64 minval,
	float64 maxval,
	int32 units,
	const std::wstring& customscalename) {
	CW2A char_devices(devices.c_str());
	CW2A char_channelname(channelname.c_str());
	CW2A char_customscalename(customscalename.c_str());

	CheckError(DAQmxCreateAOVoltageChan(task_handle,
		char_devices,
		char_channelname,
		minval,
		maxval,
		units,
		char_customscalename));
}

uInt32 CDAQmxAnalogOutTask::ConfigureBuffer(const uInt32& _sampsperchannel) {
	DBOUT(L"CDAQmxAnalogOutTask::ConfigureBuffer requested samples per channel " << _sampsperchannel);
	CheckError(DAQmxCfgOutputBuffer(task_handle, _sampsperchannel));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	uInt32 value = 0;
	CheckError(DAQmxGetBufferAttribute(task_handle, DAQmx_Buf_Output_BufSize, &value));
	DBOUT(L"CDAQmxAnalogOutTask::ConfigureBuffer buffer size " << value);
	return value;
}

uInt32 CDAQmxAnalogOutTask::ConfigureOnboardBuffer(const uInt32& _sampsperchannel) {
	CheckError(DAQmxSetBufOutputOnbrdBufSize(task_handle, _sampsperchannel));
	uInt32 bufsize = 0;
	CheckError(DAQmxGetBufOutputOnbrdBufSize(task_handle, &bufsize));
	DBOUT(L"CDAQmxAnalogOutTask::ConfigureOnboardBuffer onboard buffer size " << bufsize);
	return bufsize;
}

void CDAQmxAnalogOutTask::UseOnlyOnboardMemory(const std::wstring& _channel) {
	CW2A char_channel(_channel.c_str());
	CheckError(DAQmxSetAOUseOnlyOnBrdMem(task_handle, char_channel, true));
}

void CDAQmxAnalogOutTask::SetIdleOutputBehaviour(const std::wstring& _channel, const int32_t& _behav) {
	CW2A char_channel(_channel.c_str());
	CheckError(DAQmxSetAOIdleOutputBehavior(task_handle, char_channel, _behav));
}

void CDAQmxAnalogOutTask::SetDataTransferCondition(const std::wstring& _channel, const int32_t& _cond) {
	CW2A char_channel(_channel.c_str());
	CheckError(DAQmxSetAODataXferReqCond(task_handle, char_channel, _cond));
}

void CDAQmxAnalogOutTask::SetDataTransferMode(const std::wstring& _channel, const int32_t& _mode) {
	CW2A char_channel(_channel.c_str());
	CheckError(DAQmxSetAODataXferMech(task_handle, char_channel, _mode));
}

int32 CDAQmxAnalogOutTask::WriteAnalog(const float64* _data, int32 _sampsperchan, bool _autostart, int32 _timeout, bool32 _layout) {
	int32 writtensamples = -1;
	CheckError(DAQmxWriteAnalogF64(task_handle
		, _sampsperchan
		, _autostart
		, _timeout
		, _layout
		, _data
		, &writtensamples, NULL));
	return writtensamples;
}

void CDAQmxAnalogOutTask::WriteAnalogScalar(const float64& _value, bool _autostart, int32 _timeout) {
	CheckError(DAQmxWriteAnalogScalarF64(task_handle
		, _autostart
		, _timeout
		, _value, NULL));
}

int32 CDAQmxAnalogOutTask::WriteAnalogI16(int16* _data, int32 _sampsperchan, bool _autostart, int32 _timeout, bool32 _layout) {
	int32 writtensamples = -1;
	CheckError(DAQmxWriteBinaryI16(task_handle
		, _sampsperchan
		, _autostart
		, _timeout
		, _layout
		, _data
		, &writtensamples, NULL));
	return writtensamples;
}

void CDAQmxDigitalOutTask::CreateDOChannel(const std::wstring& _devicelines, const std::wstring& _name) {
	CW2A char_devicelines(_devicelines.c_str());
	CW2A char_name(_name.c_str());

	CheckError(DAQmxCreateDOChan(task_handle
		, char_devicelines
		, char_name
		, DAQmx_Val_ChanPerLine));
	/*
	DAQmx_Val_ChanPerLine                                             0   // One Channel For Each Line
	DAQmx_Val_ChanForAllLines                                         1   // One Channel For All Lines
	*/
}

int32 CDAQmxDigitalOutTask::WriteDigitalU8(const uInt8* _data, int32 _sampsperchan, bool _autostart, float64 _timeout, bool32 _layout) {
	int32 writtensamples = -1;
	CheckError(DAQmxWriteDigitalU8(task_handle
		, _sampsperchan
		, _autostart
		, _timeout
		, _layout
		, _data
		, &writtensamples, NULL));
	return writtensamples;
}

int32_t CDAQmxDigitalOutTask::WriteDigitalLines(const uInt8* _data, int32 _sampsperchan, bool _autostart, float64 _timeout, bool32 _layout) {
	int32 writtensamples = -1;
	CheckError(DAQmxWriteDigitalLines(task_handle
		, _sampsperchan
		, _autostart
		, _timeout
		, _layout
		, _data
		, &writtensamples, NULL));
	return writtensamples;
}

CDAQmxAnalogInTask::CDAQmxAnalogInTask() {
	mtgen.seed((uint32_t)time(NULL));
}

void CDAQmxAnalogInTask::CreateAIVoltageChannel(const std::wstring& _devicechannel,
	const std::wstring& _name,
	int32 _terminalconfig,
	float64 _minval,
	float64 _maxval,
	int32 _units,
	const std::wstring& _customscalename) {
	CW2A char_devicechannel(_devicechannel.c_str());
	CW2A char_name(_name.c_str());
	CW2A char_customscalename(_customscalename.c_str());

	CheckError(DAQmxCreateAIVoltageChan(task_handle,
		char_devicechannel,
		char_name,
		_terminalconfig,
		_minval,
		_maxval,
		_units,
		char_customscalename));
}

void CDAQmxAnalogInTask::ConfigureBuffer(const uInt32& _sampsperchannel) {
	CheckError(DAQmxCfgInputBuffer(task_handle, _sampsperchannel));
	uInt32 value = 0;
	CheckError(DAQmxGetBufferAttribute(task_handle, DAQmx_Buf_Input_BufSize, &value));
	DBOUT(L"CDAQmxAnalogInTask::ConfigureBuffer buffer size " << value);
}

int32 CDAQmxAnalogInTask::ReadU16(std::vector<uint16_t>::iterator& _databegin, std::vector<uint16_t>::iterator& _dataend, const int32& _sampsperchan, const uint32_t& _channels,  bool& _timedout, const float64& _timeout) {
	int32 readsamples = 0;
	int32_t status = DAQmxReadBinaryU16(task_handle
		, _sampsperchan
		, _timeout
		, DAQmx_Val_GroupByChannel
		, &(*_databegin)
		, static_cast<uInt32>(std::distance(_databegin, _dataend))
		, &readsamples
		, NULL);
	_timedout = (status == DAQmxErrorOperationTimedOut);
	
	if ( _timedout )			// Avoid throwing an exception on time out
		return -1;

	if ( CheckError(status) )	// CheckError could throw exception if DAQMX_THROW_EXCEPTION defined
		return -1;
	else
		return readsamples;
}

int32 CDAQmxAnalogInTask::ReadU16Dummy(std::vector<uint16_t>::iterator& _databegin, std::vector<uint16_t>::iterator& _dataend, const int32& _sampsperchan, const uint32_t& _channels,  bool& _timedout, const float64& _timeout) {
	//int32 readsamples = -1;
	_timedout = false;
	uint16_t d = 0;
	std::generate(_databegin, _dataend, [&d]() { return d++; } ); // or random: return static_cast<uint16_t>(mtgen());
	return (_sampsperchan*_channels);
}

int32 CDAQmxAnalogInTask::ReadI16(std::vector<int16_t>::iterator& _databegin, std::vector<int16_t>::iterator& _dataend, const int32& _sampsperchan, const uint32_t& _channels,  bool& _timedout, const float64& _timeout) {
	int32 readsamples = -1;
	int32_t status = DAQmxReadBinaryI16(task_handle
		, _sampsperchan
		, _timeout
		, DAQmx_Val_GroupByChannel
		, &(*_databegin)
		, static_cast<uInt32>(std::distance(_databegin, _dataend))
		, &readsamples
		, NULL);
	
	_timedout = (status == DAQmxErrorOperationTimedOut);
	
	if ( _timedout )			// Avoid throwing an exception on time out
		return -1;

	if ( CheckError(status) )
		return -1;
	else
		return readsamples;
	/*
	DAQmx_Val_GroupByChannel                                          0   // Group by Channel
	DAQmx_Val_GroupByScanNumber                                       1   // Group by Scan Number
	*/
}

void CDAQmxCounterOutTask::CreateCOChannel(const std::wstring& _counter
	, const std::wstring& _name
	, int32 _idlestate
	, float64 _initdelay
	, float64 _lowtime
	, float64 _hightime ) {
	CW2A char_counter(_counter.c_str());
	CW2A char_name(_name.c_str());

	CheckError(DAQmxCreateCOPulseChanTime(task_handle
		, char_counter
		, char_name
		, DAQmx_Val_Seconds
		, _idlestate
		, _initdelay
		, _lowtime
		, _hightime));		
}


}