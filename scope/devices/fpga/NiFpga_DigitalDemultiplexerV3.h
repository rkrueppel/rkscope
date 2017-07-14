/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_DigitalDemultiplexerV3_h__
#define __NiFpga_DigitalDemultiplexerV3_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_DigitalDemultiplexerV3_Bitfile;
 */
#define NiFpga_DigitalDemultiplexerV3_Bitfile "NiFpga_DigitalDemultiplexerV3.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_DigitalDemultiplexerV3_Signature = "9DF944060C23FE50E0AF795E74F2DFAC";

typedef enum
{
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_Acquiring = 0x22,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_InterloopFIFOoverflow = 0x80000066,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_InterloopFIFOtimeout = 0x3E,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_Onboard_Clock_Ready = 0x56,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA1Ch1 = 0x1E,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA1Ch2 = 0x12,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA2Ch1 = 0xE,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_ToHostFIFOOverflowA2Ch2 = 0xA,
   NiFpga_DigitalDemultiplexerV3_IndicatorBool_Xpoint_Switch_Ready = 0x42,
} NiFpga_DigitalDemultiplexerV3_IndicatorBool;

typedef enum
{
   NiFpga_DigitalDemultiplexerV3_IndicatorU16_LaserpulsesperpixelA1 = 0x6,
} NiFpga_DigitalDemultiplexerV3_IndicatorU16;

typedef enum
{
   NiFpga_DigitalDemultiplexerV3_ControlBool_Acq_Reset = 0x8000006A,
   NiFpga_DigitalDemultiplexerV3_ControlBool_Acquire = 0x3A,
   NiFpga_DigitalDemultiplexerV3_ControlBool_Acquirecontinuously = 0x2A,
   NiFpga_DigitalDemultiplexerV3_ControlBool_ClearInterloopFIFO = 0x32,
   NiFpga_DigitalDemultiplexerV3_ControlBool_Commit = 0x2,
   NiFpga_DigitalDemultiplexerV3_ControlBool_Countmode = 0x8000005A,
   NiFpga_DigitalDemultiplexerV3_ControlBool_Onboard_Clock_Write = 0x4E,
   NiFpga_DigitalDemultiplexerV3_ControlBool_Waitfortrigger = 0x26,
   NiFpga_DigitalDemultiplexerV3_ControlBool_Xpoint_Switch_Write = 0x46,
} NiFpga_DigitalDemultiplexerV3_ControlBool;

typedef enum
{
   NiFpga_DigitalDemultiplexerV3_ControlU8_ClockSource = 0x4A,
} NiFpga_DigitalDemultiplexerV3_ControlU8;

typedef enum
{
   NiFpga_DigitalDemultiplexerV3_ControlU16_Onboard_Clock_Write_Data = 0x52,
   NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelA1 = 0x36,
   NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelA2 = 0x1A,
   NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelAcqA1 = 0x80000062,
   NiFpga_DigitalDemultiplexerV3_ControlU16_SamplesperpixelAcqA2 = 0x8000005E,
} NiFpga_DigitalDemultiplexerV3_ControlU16;

typedef enum
{
   NiFpga_DigitalDemultiplexerV3_ControlU32_RequestedpixelsA1 = 0x2C,
   NiFpga_DigitalDemultiplexerV3_ControlU32_RequestedpixelsA2 = 0x14,
} NiFpga_DigitalDemultiplexerV3_ControlU32;

typedef enum
{
   NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea1Ch1FIFO = 3,
   NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea1Ch2FIFO = 2,
   NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea2Ch1FIFO = 1,
   NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16_ToHostArea2Ch2FIFO = 0,
} NiFpga_DigitalDemultiplexerV3_TargetToHostFifoU16;

#endif
