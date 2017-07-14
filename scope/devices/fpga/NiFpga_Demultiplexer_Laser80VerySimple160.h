/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_Demultiplexer_Laser80VerySimple160_h__
#define __NiFpga_Demultiplexer_Laser80VerySimple160_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_Demultiplexer_Laser80VerySimple160_Bitfile;
 */
#define NiFpga_Demultiplexer_Laser80VerySimple160_Bitfile "NiFpga_Demultiplexer_Laser80VerySimple160.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_Demultiplexer_Laser80VerySimple160_Signature = "9EEAB08E8198241FC9CDB22E4046EA9E";

typedef enum
{
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_Acquiring = 0x22,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_InterloopFIFOoverflow = 0x80000062,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_InterloopFIFOtimeout = 0x3E,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_Onboard_Clock_Ready = 0x56,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_ToHostFIFOOverflowA1Ch1 = 0x1E,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_ToHostFIFOOverflowA1Ch2 = 0x12,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_ToHostFIFOOverflowA2Ch1 = 0xE,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_ToHostFIFOOverflowA2Ch2 = 0xA,
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool_Xpoint_Switch_Ready = 0x42,
} NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorBool;

typedef enum
{
   NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorU16_LaserpulsesperpixelA1 = 0x6,
} NiFpga_Demultiplexer_Laser80VerySimple160_IndicatorU16;

typedef enum
{
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_Acq_Reset = 0x80000066,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_Acquire = 0x3A,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_Acquirecontinuously = 0x2A,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_ClearInterloopFIFO = 0x32,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_Commit = 0x2,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_Onboard_Clock_Write = 0x4E,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_Waitfortrigger = 0x26,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool_Xpoint_Switch_Write = 0x46,
} NiFpga_Demultiplexer_Laser80VerySimple160_ControlBool;

typedef enum
{
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU8_ClockSource = 0x4A,
} NiFpga_Demultiplexer_Laser80VerySimple160_ControlU8;

typedef enum
{
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU16_Onboard_Clock_Write_Data = 0x52,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU16_SamplesperpixelA1 = 0x36,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU16_SamplesperpixelA2 = 0x1A,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU16_SamplesperpixelAcqA1 = 0x8000005E,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU16_SamplesperpixelAcqA2 = 0x8000005A,
} NiFpga_Demultiplexer_Laser80VerySimple160_ControlU16;

typedef enum
{
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU32_RequestedpixelsA1 = 0x2C,
   NiFpga_Demultiplexer_Laser80VerySimple160_ControlU32_RequestedpixelsA2 = 0x14,
} NiFpga_Demultiplexer_Laser80VerySimple160_ControlU32;

typedef enum
{
   NiFpga_Demultiplexer_Laser80VerySimple160_TargetToHostFifoU16_ToHostArea1Ch1FIFO = 3,
   NiFpga_Demultiplexer_Laser80VerySimple160_TargetToHostFifoU16_ToHostArea1Ch2FIFO = 2,
   NiFpga_Demultiplexer_Laser80VerySimple160_TargetToHostFifoU16_ToHostArea2Ch1FIFO = 1,
   NiFpga_Demultiplexer_Laser80VerySimple160_TargetToHostFifoU16_ToHostArea2Ch2FIFO = 0,
} NiFpga_Demultiplexer_Laser80VerySimple160_TargetToHostFifoU16;

#endif
