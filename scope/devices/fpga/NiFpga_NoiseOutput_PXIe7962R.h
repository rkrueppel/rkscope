/*
 * Generated with the FPGA Interface C API Generator 12.0.0
 * for NI-RIO 12.0.0 or later.
 */

#ifndef __NiFpga_NoiseOutput_PXIe7962R_h__
#define __NiFpga_NoiseOutput_PXIe7962R_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1200
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_NoiseOutput_PXIe7962R_Bitfile;
 */
#define NiFpga_NoiseOutput_PXIe7962R_Bitfile "NiFpga_NoiseOutput_PXIe7962R.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_NoiseOutput_PXIe7962R_Signature = "AF6B7AB2245DE9B89690405679475B9A";

typedef enum
{
   NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_Acquiring = 0xE,
   NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA1Ch1Overflow = 0x1E,
   NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA1Ch2Overflow = 0x1A,
   NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA2Ch1Overflow = 0x16,
   NiFpga_NoiseOutput_PXIe7962R_IndicatorBool_ToHostA2Ch2Overflow = 0x12,
} NiFpga_NoiseOutput_PXIe7962R_IndicatorBool;

typedef enum
{
   NiFpga_NoiseOutput_PXIe7962R_ControlBool_Acquire = 0x2,
   NiFpga_NoiseOutput_PXIe7962R_ControlBool_Acquirecontinuously = 0x26,
   NiFpga_NoiseOutput_PXIe7962R_ControlBool_Quit = 0x3A,
   NiFpga_NoiseOutput_PXIe7962R_ControlBool_Waitfortrigger = 0x22,
} NiFpga_NoiseOutput_PXIe7962R_ControlBool;

typedef enum
{
   NiFpga_NoiseOutput_PXIe7962R_ControlU32_RequestedpixelsA1 = 0x28,
   NiFpga_NoiseOutput_PXIe7962R_ControlU32_RequestedpixelsA2 = 0x4,
   NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperlineA1 = 0x30,
   NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperlineA2 = 0x2C,
   NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperpixelA1 = 0x8,
   NiFpga_NoiseOutput_PXIe7962R_ControlU32_SamplesperpixelA2 = 0x34,
} NiFpga_NoiseOutput_PXIe7962R_ControlU32;

typedef enum
{
   NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA1Ch1FIFO = 3,
   NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA1Ch2FIFO = 2,
   NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA2Ch1FIFO = 1,
   NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16_ToHostA2Ch2FIFO = 0,
} NiFpga_NoiseOutput_PXIe7962R_TargetToHostFifoU16;

#endif
