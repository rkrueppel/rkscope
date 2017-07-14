/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_AnalogDemultiplexer_NI5771_h__
#define __NiFpga_AnalogDemultiplexer_NI5771_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_AnalogDemultiplexer_NI5771_Bitfile;
 */
#define NiFpga_AnalogDemultiplexer_NI5771_Bitfile "NiFpga_AnalogDemultiplexer_NI5771.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_AnalogDemultiplexer_NI5771_Signature = "CF20D93E8A5A64AF7506A904B23F975B";

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_Acquiring = 0x16,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_Configured = 0x72,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_IOModuleAIOverRange = 0x8000004A,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_InterloopFIFOoverflow = 0x80000046,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_InterloopFIFOtimeout = 0x2,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_PLLLocked = 0x6A,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_ToHostFIFOOverflowA1 = 0x12,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_ToHostFIFOOverflowA2 = 0xA,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_UserCommandIdle = 0x5A,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool_UserError = 0x6E,
} NiFpga_AnalogDemultiplexer_NI5771_IndicatorBool;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorU8_UserCommandStatus = 0x5E,
} NiFpga_AnalogDemultiplexer_NI5771_IndicatorU8;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorU32_PixelcounterA1 = 0xC,
   NiFpga_AnalogDemultiplexer_NI5771_IndicatorU32_PixelcounterA2 = 0x4,
} NiFpga_AnalogDemultiplexer_NI5771_IndicatorU32;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_ControlBool_Acquire = 0x1E,
   NiFpga_AnalogDemultiplexer_NI5771_ControlBool_Acquirecontinuously = 0x26,
   NiFpga_AnalogDemultiplexer_NI5771_ControlBool_ClearInterloopFIFO = 0x4E,
   NiFpga_AnalogDemultiplexer_NI5771_ControlBool_UserCommandCommit = 0x52,
   NiFpga_AnalogDemultiplexer_NI5771_ControlBool_Waitfortrigger = 0x1A,
} NiFpga_AnalogDemultiplexer_NI5771_ControlBool;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_ControlU8_BaselineU8Ch1 = 0x8000003A,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU8_BaselineU8Ch2 = 0x80000036,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU8_UserCommand = 0x56,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU8_UserData1 = 0x66,
} NiFpga_AnalogDemultiplexer_NI5771_ControlU8;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_ControlU16_SamplesperpixelA1 = 0x32,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU16_SamplesperpixelA2 = 0x2E,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU16_SamplesperpixelAcqA1 = 0x80000042,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU16_SamplesperpixelAcqA2 = 0x8000003E,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU16_UserData0 = 0x62,
} NiFpga_AnalogDemultiplexer_NI5771_ControlU16;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_ControlU32_RequestedpixelsA1 = 0x28,
   NiFpga_AnalogDemultiplexer_NI5771_ControlU32_RequestedpixelsA2 = 0x20,
} NiFpga_AnalogDemultiplexer_NI5771_ControlU32;

typedef enum
{
   NiFpga_AnalogDemultiplexer_NI5771_TargetToHostFifoU64_ToHostA1FIFO = 1,
   NiFpga_AnalogDemultiplexer_NI5771_TargetToHostFifoU64_ToHostA2FIFO = 0,
} NiFpga_AnalogDemultiplexer_NI5771_TargetToHostFifoU64;

#endif
