
///////////////////////////////////////////////////////////////////////////
//
// Module   : BOOT_DATA
// Part of  : EFTOS and BOOTER
// Created  : Bengt Westman
// Abstract : Header file for BOOTDATA.C
//
//         Copyright (C) 2003 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 030428  BW      Original version (based on EFTOS8 version 020712)
// 040629  ASA     Changed for GNU C and ADS 1.2 support
//
///////////////////////////////////////////////////////////////////////////

#ifndef BOOTDATA_H
#define BOOTDATA_H

#ifndef BASIC_TYPE_DEFINED
typedef unsigned char   BYTE;
typedef unsigned long   DWORD;
typedef const unsigned char* LPCSTR;
#endif  


// Pointer to the version string of the booter
#ifdef __ARMCC_VERSION
// 0x4000000
#pragma arm section zidata="Version"
#endif
extern LPCSTR BOOT_VERSION_PTR;

// Variable holding HW configuration mode value (used by e.g. HWT)
// If Mode=0 then 8  MByte SDRAM
// If Mode=1 then 16 MByte SDRAM
// If Mode=2 then 32 MByte SDRAM
#ifdef __ARMCC_VERSION
// 0x4000004
#pragma arm section zidata="HWconfig"
#endif
extern DWORD BOOT_HW_CONFIG_MODE;

// Define size of string in RAM written with pattern
// to decide type of restart that shall be performed
#define RESTART_REQ_ARR_SIZE    8

// Holds string which defines type of restart
// that has been requested "REQUEST"+n
#ifdef __ARMCC_VERSION
// 0x4000008
#pragma arm section zidata="Restart"
#endif
extern BYTE BOOT_RESTART_REQUEST[RESTART_REQ_ARR_SIZE];
#ifdef __ARMCC_VERSION
#pragma arm section zidata
#endif

#endif
