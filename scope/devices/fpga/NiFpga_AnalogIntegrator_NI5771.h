/*
 * Generated with the FPGA Interface C API Generator 14.0.0
 * for NI-RIO 14.0.0 or later.
 */

#ifndef __NiFpga_AnalogIntegrator_NI5771_h__
#define __NiFpga_AnalogIntegrator_NI5771_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1400
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_AnalogIntegrator_NI5771_Bitfile;
 */
#define NiFpga_AnalogIntegrator_NI5771_Bitfile "NiFpga_AnalogIntegrator_NI5771.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_AnalogIntegrator_NI5771_Signature = "388AC0B38E46E7AE8034AED54E95404B";

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_Acquiring = 0x42,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_Configured = 0x62,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_IOModuleAIOverRange = 0x8000000E,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_InterloopFIFOOverflow = 0x8000000A,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_InterloopFIFOTimeout = 0x22,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_PLLLocked = 0x5A,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_ToHostCh1FIFOOverflow = 0x2A,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_ToHostCh2FIFOOverflow = 0x26,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_UserCommandIdle = 0x4A,
   NiFpga_AnalogIntegrator_NI5771_IndicatorBool_UserError = 0x5E,
} NiFpga_AnalogIntegrator_NI5771_IndicatorBool;

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_IndicatorU8_UserCommandStatus = 0x4E,
} NiFpga_AnalogIntegrator_NI5771_IndicatorU8;

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_IndicatorU32_Pixelcounter = 0x2C,
} NiFpga_AnalogIntegrator_NI5771_IndicatorU32;

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_ControlBool_Acquire = 0x36,
   NiFpga_AnalogIntegrator_NI5771_ControlBool_Acquirecontinuously = 0x3A,
   NiFpga_AnalogIntegrator_NI5771_ControlBool_ClearInterloopFIFO = 0x1E,
   NiFpga_AnalogIntegrator_NI5771_ControlBool_UserCommandCommit = 0x1A,
   NiFpga_AnalogIntegrator_NI5771_ControlBool_Waitfortrigger = 0x32,
} NiFpga_AnalogIntegrator_NI5771_ControlBool;

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_ControlU8_Baselinex8Ch1 = 0x80000006,
   NiFpga_AnalogIntegrator_NI5771_ControlU8_Baselinex8Ch2 = 0x80000002,
   NiFpga_AnalogIntegrator_NI5771_ControlU8_UserCommand = 0x46,
   NiFpga_AnalogIntegrator_NI5771_ControlU8_UserData1 = 0x56,
} NiFpga_AnalogIntegrator_NI5771_ControlU8;

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_ControlU16_UserData0 = 0x52,
} NiFpga_AnalogIntegrator_NI5771_ControlU16;

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_ControlU32_Requestedpixels = 0x3C,
   NiFpga_AnalogIntegrator_NI5771_ControlU32_Samplesperline = 0x80000014,
   NiFpga_AnalogIntegrator_NI5771_ControlU32_Samplesperpixel = 0x80000010,
} NiFpga_AnalogIntegrator_NI5771_ControlU32;

typedef enum
{
   NiFpga_AnalogIntegrator_NI5771_TargetToHostFifoU32_ToHostCh1FIFO = 1,
   NiFpga_AnalogIntegrator_NI5771_TargetToHostFifoU32_ToHostCh2FIFO = 0,
} NiFpga_AnalogIntegrator_NI5771_TargetToHostFifoU32;

#endif
