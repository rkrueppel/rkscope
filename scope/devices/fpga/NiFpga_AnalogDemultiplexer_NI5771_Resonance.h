/*
 * Generated with the FPGA Interface C API Generator 14.0.0
 * for NI-RIO 14.0.0 or later.
 */

#ifndef __NiFpga_AnalogDemultiplexer_NI5771_Resonance_h__
#define __NiFpga_AnalogDemultiplexer_NI5771_Resonance_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1400
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_AnalogDemultiplexer_NI5771_Resonance_Bitfile;
 */
#define NiFpga_AnalogDemultiplexer_NI5771_Resonance_Bitfile "NiFpga_AnalogDemultiplexer_NI5771_Resonance.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_AnalogDemultiplexer_NI5771_Resonance_Signature = "6C6585DECF6FCE1E43A45367D1ED1D9D";

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_Acquiring = 0x3A,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_Configured = 0x6E,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_IOModuleAIOverRange = 0x8000001A,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_InterloopFIFOOverflow = 0x8000000A,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_InterloopFIFOtimeout = 0x32,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_PLLLocked = 0x66,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_ToHostA1FIFOOverflow = 0x26,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_ToHostA2FIFOOverflow = 0x22,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_UserCommandIdle = 0x56,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool_UserError = 0x6A,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorBool;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorU8_UserCommandStatus = 0x5A,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorU8;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorU32_PixelcounterA1 = 0x2C,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorU32_PixelcounterA2 = 0x28,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_IndicatorU32;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlBool_Acquire = 0x42,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlBool_Acquirecontinuously = 0x4A,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlBool_ClearInterloopFIFO = 0x36,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlBool_UserCommandCommit = 0x1E,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlBool_Waitfortrigger = 0x3E,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlBool;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU8_BaselineU8Ch1 = 0x80000012,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU8_BaselineU8Ch2 = 0x8000000E,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU8_CutoffU8Ch1 = 0x80000006,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU8_CutoffU8Ch2 = 0x80000002,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU8_UserCommand = 0x52,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU8_UserData1 = 0x62,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU8;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU16_Samplesperpixel = 0x80000016,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU16_UserData0 = 0x5E,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU16;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU32_RequestedpixelsA1 = 0x4C,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU32_RequestedpixelsA2 = 0x44,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_ControlU32;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_TargetToHostFifoU64_ToHostA1FIFO = 1,
   NiFpga_AnalogDemultiplexer_NI5771_Resonance_TargetToHostFifoU64_ToHostA2FIFO = 0,
} NiFpga_AnalogDemultiplexer_NI5771_Resonance_TargetToHostFifoU64;

#endif
