/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_PhotonCounterV2_h__
#define __NiFpga_PhotonCounterV2_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_PhotonCounterV2_Bitfile;
 */
#define NiFpga_PhotonCounterV2_Bitfile "NiFpga_PhotonCounterV2.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_PhotonCounterV2_Signature = "697C5713958CBD623488103665FF5107";

typedef enum
{
   NiFpga_PhotonCounterV2_IndicatorBool_Acquiring = 0x12,
   NiFpga_PhotonCounterV2_IndicatorBool_InterloopFIFOoverflow = 0x80000032,
   NiFpga_PhotonCounterV2_IndicatorBool_InterloopFIFOtimeout = 0x1E,
   NiFpga_PhotonCounterV2_IndicatorBool_Onboard_Clock_Ready = 0x42,
   NiFpga_PhotonCounterV2_IndicatorBool_ToHostFIFOOverflowCh1 = 0x22,
   NiFpga_PhotonCounterV2_IndicatorBool_ToHostFIFOOverflowCh2 = 0x2,
   NiFpga_PhotonCounterV2_IndicatorBool_Xpoint_Switch_Ready = 0x52,
} NiFpga_PhotonCounterV2_IndicatorBool;

typedef enum
{
   NiFpga_PhotonCounterV2_ControlBool_Acq_Reset = 0x80000036,
   NiFpga_PhotonCounterV2_ControlBool_Acquire = 0x26,
   NiFpga_PhotonCounterV2_ControlBool_Acquirecontinuously = 0xA,
   NiFpga_PhotonCounterV2_ControlBool_ClearInterloopFIFOs = 0x3E,
   NiFpga_PhotonCounterV2_ControlBool_Commit = 0x56,
   NiFpga_PhotonCounterV2_ControlBool_Countingmode = 0x6,
   NiFpga_PhotonCounterV2_ControlBool_Onboard_Clock_Write = 0x46,
   NiFpga_PhotonCounterV2_ControlBool_Waitfortrigger = 0x16,
   NiFpga_PhotonCounterV2_ControlBool_Xpoint_Switch_Write = 0x4E,
} NiFpga_PhotonCounterV2_ControlBool;

typedef enum
{
   NiFpga_PhotonCounterV2_ControlU8_ClockSource = 0x4A,
} NiFpga_PhotonCounterV2_ControlU8;

typedef enum
{
   NiFpga_PhotonCounterV2_ControlU16_Onboard_Clock_Write_Data = 0x3A,
   NiFpga_PhotonCounterV2_ControlU16_Samplesperpixel = 0x1A,
   NiFpga_PhotonCounterV2_ControlU16_SamplesperpixelAcq = 0x8000002E,
} NiFpga_PhotonCounterV2_ControlU16;

typedef enum
{
   NiFpga_PhotonCounterV2_ControlU32_Requestedpixels = 0xC,
   NiFpga_PhotonCounterV2_ControlU32_Samplesperline = 0x80000028,
} NiFpga_PhotonCounterV2_ControlU32;

typedef enum
{
   NiFpga_PhotonCounterV2_TargetToHostFifoU16_ToHostCh1FIFO = 1,
   NiFpga_PhotonCounterV2_TargetToHostFifoU16_ToHostCh2FIFO = 0,
} NiFpga_PhotonCounterV2_TargetToHostFifoU16;

#endif
