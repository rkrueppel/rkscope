/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_AnalogDemultiplexerV2_NI5771_h__
#define __NiFpga_AnalogDemultiplexerV2_NI5771_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_AnalogDemultiplexerV2_NI5771_Bitfile;
 */
#define NiFpga_AnalogDemultiplexerV2_NI5771_Bitfile "NiFpga_AnalogDemultiplexerV2_NI5771.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_AnalogDemultiplexerV2_NI5771_Signature = "9C64626794B6859E042752A0A66A1B22";

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_Acquiring = 0x36,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_Configured = 0x6E,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_IOModuleAIOverRange = 0x8000001A,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_InterloopFIFOOverflow = 0x8000000A,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_InterloopFIFOtimeout = 0x32,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_PLLLocked = 0x66,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_ToHostA1FIFOOverflow = 0x26,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_ToHostA2FIFOOverflow = 0x22,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_UserCommandIdle = 0x56,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool_UserError = 0x6A,
} NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorBool;

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorU8_UserCommandStatus = 0x5A,
} NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorU8;

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorU32_PixelcounterA1 = 0x2C,
   NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorU32_PixelcounterA2 = 0x28,
} NiFpga_AnalogDemultiplexerV2_NI5771_IndicatorU32;

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Acquire = 0x3E,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Acquirecontinuously = 0x46,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_ClearInterloopFIFO = 0x1E,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_UserCommandCommit = 0x4E,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool_Waitfortrigger = 0x3A,
} NiFpga_AnalogDemultiplexerV2_NI5771_ControlBool;

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_BaselineU8Ch1 = 0x80000012,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_BaselineU8Ch2 = 0x8000000E,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_CutoffU8Ch1 = 0x80000006,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_CutoffU8Ch2 = 0x80000002,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_UserCommand = 0x52,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8_UserData1 = 0x62,
} NiFpga_AnalogDemultiplexerV2_NI5771_ControlU8;

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU16_Samplesperpixel = 0x80000016,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU16_UserData0 = 0x5E,
} NiFpga_AnalogDemultiplexerV2_NI5771_ControlU16;

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU32_RequestedpixelsA1 = 0x48,
   NiFpga_AnalogDemultiplexerV2_NI5771_ControlU32_RequestedpixelsA2 = 0x40,
} NiFpga_AnalogDemultiplexerV2_NI5771_ControlU32;

typedef enum
{
   NiFpga_AnalogDemultiplexerV2_NI5771_TargetToHostFifoU64_ToHostA1FIFO = 1,
   NiFpga_AnalogDemultiplexerV2_NI5771_TargetToHostFifoU64_ToHostA2FIFO = 0,
} NiFpga_AnalogDemultiplexerV2_NI5771_TargetToHostFifoU64;

#endif
