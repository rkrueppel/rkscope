/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_ResonanceScanner_h__
#define __NiFpga_ResonanceScanner_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_ResonanceScanner_Bitfile;
 */
#define NiFpga_ResonanceScanner_Bitfile "NiFpga_ResonanceScanner.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_ResonanceScanner_Signature = "E9A50DB9A6288EA55E3473994CC9BF69";

typedef enum
{
   NiFpga_ResonanceScanner_IndicatorBool_Acquiring = 0x3E,
   NiFpga_ResonanceScanner_IndicatorBool_Configured = 0x4E,
   NiFpga_ResonanceScanner_IndicatorBool_IOModuleAIOverRange = 0x42,
   NiFpga_ResonanceScanner_IndicatorBool_InterloopFIFOOverflow = 0x3A,
   NiFpga_ResonanceScanner_IndicatorBool_InterloopFIFOTimeout = 0x1A,
   NiFpga_ResonanceScanner_IndicatorBool_Pixelfinished = 0xA,
   NiFpga_ResonanceScanner_IndicatorBool_ResonanceTrigger = 0xE,
   NiFpga_ResonanceScanner_IndicatorBool_ToHostCh1FIFOOverflow = 0x22,
   NiFpga_ResonanceScanner_IndicatorBool_ToHostCh2FIFOOverflow = 0x1E,
} NiFpga_ResonanceScanner_IndicatorBool;

typedef enum
{
   NiFpga_ResonanceScanner_IndicatorU32_Pixelcounter = 0x24,
} NiFpga_ResonanceScanner_IndicatorU32;

typedef enum
{
   NiFpga_ResonanceScanner_ControlBool_Acquire = 0x2E,
   NiFpga_ResonanceScanner_ControlBool_Acquirecontinuously = 0x32,
   NiFpga_ResonanceScanner_ControlBool_ClearInterloopFIFO = 0x2,
   NiFpga_ResonanceScanner_ControlBool_Waitfortrigger = 0x2A,
} NiFpga_ResonanceScanner_ControlBool;

typedef enum
{
   NiFpga_ResonanceScanner_ControlI16_Scannerdelay = 0x6,
} NiFpga_ResonanceScanner_ControlI16;

typedef enum
{
   NiFpga_ResonanceScanner_ControlU16_Baselinex8Ch1 = 0x16,
   NiFpga_ResonanceScanner_ControlU16_Baselinex8Ch2 = 0x12,
} NiFpga_ResonanceScanner_ControlU16;

typedef enum
{
   NiFpga_ResonanceScanner_ControlU32_Requestedpixels = 0x34,
   NiFpga_ResonanceScanner_ControlU32_Samplesperline = 0x48,
   NiFpga_ResonanceScanner_ControlU32_Samplesperpixel = 0x44,
} NiFpga_ResonanceScanner_ControlU32;

typedef enum
{
   NiFpga_ResonanceScanner_TargetToHostFifoU32_ToHostCh1FIFO = 1,
   NiFpga_ResonanceScanner_TargetToHostFifoU32_ToHostCh2FIFO = 0,
} NiFpga_ResonanceScanner_TargetToHostFifoU32;

#endif
