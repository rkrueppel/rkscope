#pragma once

// Include and link National Instruments DAQmx library
#include <NIDAQmx.h>
#pragma comment(lib, "NIDAQmx.lib")

#include "helpers/ScopeDatatypes.h"

/** This is basically a C++ wrapper around NI's %DAQmx C library. Contains all DAQmx related stuff. */
namespace DAQmx {

	/** Checks return value of NI DAQmx function for error, prints out error information and throws an
	* exception if DAQMX_THROW_EXCEPTION is defined true
	* @return returns true if an error occured, false if no error */
	bool CheckError(const int32& error);

	/** Predicts the actual sampling rate.
	* The real sample rate for analog in and out operations can differ slightly. 
	* Read NI Knowledge Base "How Is the Actual Scan Rate Determined When I Specify the Scan Rate for My Data Acquisition?".\n
	* see here: http://digital.ni.com/public.nsf/websearch/5782F1B396474BAF86256A1D00572D6E?OpenDocument \n
	* and here: http://digital.ni.com/public.nsf/allkb/4BBE1409700F6CE686256E9200652F6B?OpenDocument \n
	* for details of the calculations.\n
	* Note that the result of this function does not take into account the maximum sample rate of your device, this can only be looked up in the devices
	* data sheet!!!!
	* For NI-6259: 32 bit AI and AO clocks.\n
	* @param[in] _desiredrate desired sample rate
	* @param[in] _nochannels number of channels
	* @param[in] _refclockrate rate of the reference clock
	* @param[in] _mode Modes for PredictSampleRate (defined in NIDAQmx.h)\n
	* - DAQmx_Val_AI                                                      10100 // Analog Input\n
	* - DAQmx_Val_AO                                                      10102 // Analog Output
	* @return the actual sampling rate the device will use */
	double PredictSampleRate(const double& _desiredrate, const uint32_t& _nochannels, const double& _refclockrate, const int32& _mode); 

	/** Calculates a sampling rate which both analog in and out can comply to.
	* see here: http://digital.ni.com/public.nsf/websearch/5782F1B396474BAF86256A1D00572D6E?OpenDocument \n
	* for an explanation why both rates could be different.\n
	* Note that the result of this function does not take into account the maximum sample rate of your device, this can only be looked up in the devices
	* data sheet!!!!
	* @param[in] _desiredrate desired sampling rate (in Hz)
	* @param[in] _nochannelsin the number of channels in the input task
	* @param[in] _nochannelsout the number of channels in the output task
	* @param[in] _refclockrate frequency of the reference clock (in Hz)
	* @return the sampling rate (in Hz) which both input and output can have simultaneously */
	double CoerceSampleRates(const double& _desiredrate, const uint32_t& _nochannelsin, const uint32_t& _nochannelsout, const double& _refclockrate);

	/** Generates a string for the sample clock from daq_timing.
	* @param _timing choosen timing mode (onboard, reference clock, external clock)
	* @param _externalclocksource source of an external sample clock
	* @return a string with the correct clock stuff you can provide to ConfigureSampleTiming */
	std::wstring ClockString(const scope::DaqTimingHelper::Mode& _timing, const std::wstring& _externalclocksource);

	/** Base class for DAQmx tasks */
	class CDAQmxTask {

		protected:
			/** the DAQmx task handle */
			TaskHandle task_handle;

		public:
			CDAQmxTask();

			/** Clear the task */
			~CDAQmxTask(void);

			/** @param[in] _name name of the task, leave empty */
			void CreateTask(const std::wstring& _name = L"");

			/** @return task already created? */
			bool Created() const;

			/** Configures implicit timing for the task
			* @param[in] _mode Timing mode, possible values (from NIDAQmx.h) are DAQmx_Val_FiniteSamps or DAQmx_Val_ContSamps
			* @param[in] _samplesperchan how many samples per channel are expected */
			void ConfigureImplicitTiming(const int32& _mode, const int32& _samplesperchan);

			/** Configures a sample timing for the task
			* @param[in] _src Source terminal of the sample clock, or L"" for onboard PPL (locked to a reference clock) or L"OnboardClock" for onboard derived clock generation
			* @param[in] _rate Desired sample rate
			* @param[in] _samplesperchan Estimated samples per channels, defines buffer size
			* @param[in] _samplingtype Can be either DAQmx_Val_FiniteSamps or DAQmx_Val_ContSamps
			* @param[in] _actedge Active edge of sample clock, can be either DAQmx_Val_Rising or DAQmx_Val_Falling */
			void ConfigureSampleTiming(const std::wstring& _src, const float64& _rate, const int32& _samplesperchan, const int32& _samplingtype = DAQmx_Val_ContSamps, const int32& _actedge = DAQmx_Val_Rising);

			/** Configures the reference clock for the task
			* @param[in] _src source terminal of the reference clock
			* @param[in] _rate rate of the reference clock (has to be known!) */
			void ConfigureReferenceClock(const std::wstring& _src, const float64& _rate);

			/** Configures the start trigger
			* @param[in] _src terminal the start trigger is connected to
			* @param[in] _trigedge either DAQmx_Val_Rising or DAQmx_Val_Falling */
			void ConfigureDigStartTrigger(const std::wstring& _src, const int32& _trigedge = DAQmx_Val_Rising);

			/** Starts the task */
			void Start(void);
			
			/** Stops the task */
			void Stop(void);

			/** Clear the task */
			void Clear(void);

			/** @return is the task already done? */
			bool IsTaskDone(void);

			/** Waits until the task is done
			* @param[in] _waittime wait maximally _waittime
			* @return 0 if task done, a positive value is a warning, a negative value an error */
			int32_t WaitUntilDone(const float64& _waittime);

			/** Exports a signal to an output terminal
			* @param[in] _signal Allowed signal are (from NiDAQmx.h):\n
			* - DAQmx_Val_AIConvertClock                                          12484 // AI Convert Clock\n
			* - DAQmx_Val_10MHzRefClock                                           12536 // 10MHz Reference Clock\n
			* - DAQmx_Val_20MHzTimebaseClock                                      12486 // 20MHz Timebase Clock\n
			* - DAQmx_Val_SampleClock                                             12487 // Sample Clock\n
			* - DAQmx_Val_AdvanceTrigger                                          12488 // Advance Trigger\n
			* - DAQmx_Val_ReferenceTrigger                                        12490 // Reference Trigger\n
			* - DAQmx_Val_StartTrigger                                            12491 // Start Trigger\n
			* - DAQmx_Val_AdvCmpltEvent                                           12492 // Advance Complete Event\n
			* - DAQmx_Val_AIHoldCmpltEvent                                        12493 // AI Hold Complete Event\n
			* - DAQmx_Val_CounterOutputEvent                                      12494 // Counter Output Event\n
			* - DAQmx_Val_ChangeDetectionEvent                                    12511 // Change Detection Event\n
			* - DAQmx_Val_WDTExpiredEvent                                         12512 // Watchdog Timer Expired Event\n
			* @param[in] _terminal the device terminal to export to */
			void ExportSignal(int32 _signal, const std::wstring& _terminal);

			/** Sets the write offset relative to the first buffer position */
			void SetWriteOffset(int32 posreltofirst);

			/** Sets the mode of sample regeneration */
			void SetRegeneration(const bool& _regenerate);
	};

	/** Wraps a DAQmx Analog Output task */
	class CDAQmxAnalogOutTask
		: public CDAQmxTask {

		public:
			/** Creates an analog output channel
			* Allowed values for terminal configuration (from NiDAQmx.h):\n
			* Values for DAQmx_PhysicalChan_AI_TermCfgs and DAQmx_PhysicalChan_AO_TermCfgs\n
			* Value set TerminalConfigurationBits\n
			* - DAQmx_Val_Bit_TermCfg_RSE                                         (1<<0) // RSE terminal configuration\n
			* - DAQmx_Val_Bit_TermCfg_NRSE                                        (1<<1) // NRSE terminal configuration\n
			* - DAQmx_Val_Bit_TermCfg_Diff                                        (1<<2) // Differential terminal configuration\n
			* - DAQmx_Val_Bit_TermCfg_PseudoDIFF                                  (1<<3) // Pseudodifferential terminal configuration\n */
			void CreateAOVoltageChannel(const std::wstring& _devices,
				const std::wstring& _channelname = L"",
				float64 _minval = -10.0,
				float64 _maxval = 10.0,
				int32 _units = DAQmx_Val_Volts,
				const std::wstring& _customscalename = L"");

			/** Configure the daqmx output buffer on the PC
			* @param[in] _sampsperchannel buffer size is _sampsperchannel times number of channels
			* @return the actual buffer size */
			uInt32 ConfigureBuffer(const uInt32& _sampsperchannel);

			/** Configure the output buffer on the device hardware
			* @param[in] _sampsperchannel buffer size is _sampsperchannel times number of channels
			* @return the actual buffer size */
			uInt32 ConfigureOnboardBuffer(const uInt32& _sampsperchannel);

			/** Configures the task to use only the devices' onboard memory. Write operations do not use a DAQmx buffer on the PC but write directly to onboard memory.
			* @warning You cannot write to the device after the start was started. This is only possible with DAQmx buffer on the PC.
			* @param[in] _channel The channel(s) for which this is configured */
			void UseOnlyOnboardMemory(const std::wstring& _channel);

			/** Configures the output behaviour when the task is not running
			* @param[in] _channel the channel(s) for which this is configured
			* @param[in] _behav the desired behaviour
			* - DAQmx_Val_ZeroVolts					output 0V continuously
			* - DAQmx_Val_HighImpedance				set output to a high-impedance state, effectively disconnecting output circuitry from IO connector
			* - DAQmx_Val_MaintainExistingValue		output the last generated sample continously */
			void SetIdleOutputBehaviour(const std::wstring& _channel, const int32_t& _behav);

			/** Configures the condition for data transfer to the device
			* @param[in] _channel the channel(s) for which this is configured
			* @param[in] _cond the transfer condition
			* - DAQmx_Val_OnBrdMemEmpty				Transfer data to the device only when there is no data in the onboard memory of the device.
			* - DAQmx_Val_OnBrdMemHalfFullOrLess	Transfer data to the device any time the onboard memory is less than half full.
			* - DAQmx_Val_OnBrdMemNotFull			Transfer data to the device any time the onboard memory of the device is not full. */
			void SetDataTransferCondition(const std::wstring& _channel, const int32_t& _cond);

			/** Configures the mode of data transfer between the daqmx buffer on the PC and the onboard buffer of the device
			* @param[in] _channel the channel(s) for which this is configured
			* @param[in] _mode the transfer mode
			* - DAQmx_Val_DMA						Direct Memory Access. Data transfers take place independently from the application.
			* - DAQmx_Val_Interrupts				Data transfers take place independently from the application. Using interrupts increases CPU usage because the CPU must service interrupt requests. Typically, you should use interrupts if the device is out of DMA channels.
			* - DAQmx_Val_ProgrammedIO				Data transfers take place when you call an NI-DAQmx Read function or an NI-DAQmx Write function.
			* - DAQmx_Val_USBbulk					Data transfers take place independently from the application using a USB bulk pipe. */
			void SetDataTransferMode(const std::wstring& _channel, const int32_t& _mode);

				/** Writes several samples to the buffer of an analog output task. _data array should be grouped by channel and contain at least _sampsperchan times number of channels values.
			* @param[in] _data pointer to a float64 array, whose values should fall into the configured output range of the task.
			* @param[in] _sampsperchan number of samples per channel to be written.
			* @param[in] _autostart true if task should be automatically started after write to buffer
			* @param[in] _timeout how long to wait for the write to the buffer to happen, in seconds
			* @param[in] _layout data layout to use
			* - DAQmx_Val_GroupByChannel		by channel (AAAAABBBBBCCCCC)
			* - DAQmx_Val_GroupByScanNumber		by sample (ABCABCABCABCABC) */
			int32 WriteAnalog(const float64* _data, int32 _sampsperchan, bool _autostart = false, int32 _timeout = 2, bool32 _layout = DAQmx_Val_GroupByChannel);

			/** Writes a single sample to the buffer of an analog output task.
			* @param[in] _value should fall into the configured output range of the task
			* @param[in] _autostart true if task should be automatically started after write to buffer
			* @param[in] _timeout how long to wait for the write to the buffer to happen, in seconds */
			void WriteAnalogScalar(const float64& _value, bool _autostart = false, int32 _timeout = 1000);

			/** Writes several samples to the buffer of an analog output task. _data array should be grouped by channel and contain at least _sampsperchan times number of channels values.
			* @param[in] _data pointer to a int16 array, whose values cover the configured output range of the task.
			* @param[in] _sampsperchan number of samples per channel to be written.
			* @param[in] _autostart true if task should be automatically started after write to buffer
			* @param[in] _timeout how long to wait for the write to the buffer to happen, in seconds
			* @param[in] _layout data layout to use
			* - DAQmx_Val_GroupByChannel		by channel (AAAAABBBBBCCCCC)
			* - DAQmx_Val_GroupByScanNumber		by sample (ABCABCABCABCABC) */
			int32 WriteAnalogI16(int16* _data, int32 _sampsperchan, bool _autostart = false, int32 _timeout = 2, bool32 _layout = DAQmx_Val_GroupByChannel);
	};

	/** Wraps a DAQmx Digital Output task. */
	class CDAQmxDigitalOutTask
		: public CDAQmxTask {

		public:
			/** Creates a digital output channel. The task uses one channel for each line.
			* @param[in] _devicelines string with the device output lines to use
			* @param[in] _channelname name of the task */
			void CreateDOChannel(const std::wstring& _devicelines, const std::wstring& _channelname = L"");

			/** Writes to an 8 bit digital output port, lowest bit is line0, highest bit is line7.
			* @param[in] _data pointer to a uint8 array
			* @param[in] _sampsperchan number of samples per channel to be written.
			* @param[in] _autostart true if task should be automatically started after write to buffer
			* @param[in] _timeout how long to wait for the write to the buffer to happen, in seconds
			* @param[in] _layout data layout to use
			* - DAQmx_Val_GroupByChannel		by channel (AAAAABBBBBCCCCC)
			* - DAQmx_Val_GroupByScanNumber		by sample (ABCABCABCABCABC) */
			int32 WriteDigitalU8(const uInt8* _data, int32 _sampsperchan, bool _autostart = false, float64 _timeout = 2, bool32 _layout = DAQmx_Val_GroupByChannel);

			/** Writes to up to 8 digital lines (in one port, I suppose), depending on which lines are configured into the channel.
			* @param[in] _data pointer to a uint8 array
			* @param[in] _sampsperchan number of samples per channel to be written.
			* @param[in] _autostart true if task should be automatically started after write to buffer
			* @param[in] _timeout how long to wait for the write to the buffer to happen, in seconds
			* @param[in] _layout data layout to use
			* - DAQmx_Val_GroupByChannel		by channel (AAAAABBBBBCCCCC)
			* - DAQmx_Val_GroupByScanNumber		by sample (ABCABCABCABCABC) */
			int32_t WriteDigitalLines(const uInt8* _data, int32 _sampsperchan, bool _autostart = false, float64 _timeout = 2, bool32 _layout = DAQmx_Val_GroupByChannel);
	};


	/** Wraps a DAQmx Analog Input Task.
	* Usually the device has int16 internally (since the ranges are +-x volts)
	* If one, however, reads that out as a uin16, positive data is from 0-32767 and all negative data is 0 */
	class CDAQmxAnalogInTask
		: public CDAQmxTask {

		protected:
			/** For the ReadDummy function which returns random numbers */
			std::mersenne_twister<uint32_t, 32, 624, 397, 31, 0x9908b0df, 11, 7, 0x9d2c5680, 15, 0xefc60000, 18> mtgen;

		public:
			CDAQmxAnalogInTask();

			/** @param[in] _devicechannel channel names on the device
			* @param[in] _name name this channel should have
			* @param[in] _terminalconfig Input terminal configuration. Possible values are (from NiDAQmx.h):\n
			* - DAQmx_Val_Bit_TermCfg_RSE				(1<<0) // RSE terminal configuration\n
			*	- DAQmx_Val_Bit_TermCfg_NRSE			(1<<1) // NRSE terminal configuration\n
			*	- DAQmx_Val_Bit_TermCfg_Diff			(1<<2) // Differential terminal configuration\n
			*	- DAQmx_Val_Bit_TermCfg_PseudoDIFF		(1<<3) // Pseudodifferential terminal configuration\n
			* @param[in] _minval,_maxval minimum and maximum expected input signal (changes the digitizer preamp, so choose wisely)
			* @param[in] _units Unit type of the input signal. Possible values are (from NiDAQmx.h):\n
			* - DAQmx_Val_Volts						10348 // Volts\n
			* - DAQmx_Val_FromCustomScale				10065 // From Custom Scale\n
			* @param[in] _customscalename name of a custom defined scale */
			void CreateAIVoltageChannel(const std::wstring& _devicechannel,
				const std::wstring& _name = L"",
				int32 _terminalconfig = DAQmx_Val_Cfg_Default ,
				float64 _minval = -10.0,
				float64 _maxval = 10.0,
				int32 _units = DAQmx_Val_Volts,
				const std::wstring& _customscalename = L"");

			/** Configures input buffer (there is one buffer per channel)
			* @param[in] _sampsperchannel buffersize per channel */
			void ConfigureBuffer(const uInt32& _sampsperchannel);

			
			/** Reads from a device as signed 16bit integers
			* @param[in,out] _databegin iterator of uint16_t vector where read into should start
			* @param[in,out] _dataend iterator of uint16_t vector where read into should end
			* @param[in] _sampsperchan number of samples to read per channel
			* @param[in] _channels number of channels to read from
			* @param[out] _timedout true if read timedout (then also returning -1)
			* @param[in] _timeout in seconds, wait that long for enough samples to be in the buffer
			* @return number of samples read, or -1 in case of an error
			* @warning size of _data has to be at least _sampsperchan*_channels
			* @warning time out error does never throw an exception
			* @exception scope::ScopeException on error during read, except time out error */
			int32 ReadU16(std::vector<uint16_t>::iterator& _databegin, std::vector<uint16_t>::iterator& _dataend, const int32& _sampsperchan, const uint32_t& _channels, bool& _timedout, const float64& _timeout = 2);

			/** Generates random numbers or zeros instead of real read from a device
			* @param[in,out] _databegin iterator of uint16_t vector where generation starts
			* @param[in,out] _dataend iterator of uint16_t vector where generation ends
			* @param[in] _sampsperchan number of samples to generate per channel
			* @param[in] _channels number of channels to read from
			* @param[out] _timedout true if read timedout (then also returning -1)
			* @param[in] _timeout in seconds, wait that long for enough samples to be in the buffer
			* @return number of samples read, or -1 in case of an error
			* @warning size of _data has to be at least _sampsperchan*_channels */
			int32 ReadU16Dummy(std::vector<uint16_t>::iterator& _databegin, std::vector<uint16_t>::iterator& _dataend, const int32& _sampsperchan, const uint32_t& _channels,  bool& _timedout, const float64& _timeout = 2);

			/** Reads from a device as signed 16bit integers
			* @param[in,out] _databegin iterator of int16_t vector where read into should start
			* @param[in,out] _dataend iterator of int16_t vector where read into should end
			* @param[in] _sampsperchan number of samples to read per channel
			* @param[in] _channels number of channels to read from
			* @param[out] _timedout true if read timedout (then also returning -1)
			* @param[in] _timeout wait that long for enough samples to be in the buffer
			* @return number of samples read, or -1 in case of an error
			* @warning size of _data has to be at least _sampsperchan*_channels
			* @warning time out error does never throw an exception
			* @exception scope::ScopeException on error during read, except time out error */
			int32 ReadI16(std::vector<int16_t>::iterator& _databegin, std::vector<int16_t>::iterator& _dataend, const int32& _sampsperchan, const uint32_t& _channels,  bool& _timedout, const float64& _timeout = 2);
	};

	/** Wraps a DAQmx Counter Output task. */
	class CDAQmxCounterOutTask
		: public CDAQmxTask {

		public:
			/** Creates a counter output task
			* @param[in] _counter name of the counter to use
			* @param[in] _name name to assign to the created channel
			* @param[in] _idlestate resting state of output terminal
			* - DAQmx_Val_High		10192 // High
			* - DAQmx_Val_Low			10214 // Low
			* @param[in] _initdelay time in seconds to wait before the first pulse
			* @param[in] _lowtime time in seconds between pulses (low state)
			* @param[in] _hightime time in seconds of the pulse (high state) */
			void CreateCOChannel(const std::wstring& _counter
				, const std::wstring& _name = L""
				, int32 _idlestate = 0
				, float64 _initdelay = 0
				, float64 _lowtime = 0.1
				, float64 _hightime = 0.1);
	};

}