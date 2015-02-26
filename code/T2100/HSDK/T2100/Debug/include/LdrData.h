
///////////////////////////////////////////////////////////////////////////
//
// Module   : LDRDATA
// Part of  : EFTOS and LOADER
// Created  : Bengt Westman
// Abstract : Header file for LDRDATA
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
// 030428  BW      Original version (based on EFTOS8 version 030409)
// 030613  BW      Variable LDR_LanAddr has been removed.
// 030909  BW      Added variables LDR_APPL_RELOAD_PARAM and
//                 parameter LDR_APPL_UPDATE_B
// 031102  JB      Added DSP_SavedContrast variable
// 040629  ASA     Changed for GNU C support
//
//
///////////////////////////////////////////////////////////////////////////

#ifndef LDRDATA_H
#define LDRDATA_H

#ifndef BASIC_TYPE_DEFINED
typedef unsigned char   BYTE;
typedef unsigned int    BOOL;
typedef unsigned long   DWORD;
typedef const unsigned char* LPCSTR;
typedef DWORD (*FUNCPTR)();
#endif


#ifdef __ARMCC_VERSION
#pragma arm section zidata="ldrdata"
#endif

// Pointer to where execution shall start after
// Loader has completed execution
extern FUNCPTR LDR_START_PTR;

// Flag indicating that an application is loaded okay.
extern BOOL LDR_APPL_B;

// Define size of check area in RAM written with pattern
// to detect if RAM has lost power (this value must be the same
// as CHECK_AREA_SIZE in AppData.h)
#define CHECK_AREA_SIZE         16

// Check area in RAM written with pattern
extern BYTE LDR_CheckArea[CHECK_AREA_SIZE];

// Computed checksum of the loaded application code
extern DWORD LDR_APPL_CHECKSUM;

// Start and stop addresses of the checksummed area of the 
// application code
extern DWORD LDR_LOW_DEST_ADDR;
extern DWORD LDR_HIGH_DEST_ADDR;

// Pointer to the loader version string. Initiated by
// the loader.
extern LPCSTR LDR_VersionPtr;

// Define size of battery test area in RAM
#define BAT_AREA_SIZE           9

// Area used by the hardware testprogram to test battery backup
extern BYTE LDR_BatTestArea[BAT_AREA_SIZE];

// Parameter provided by Application at Reload request
extern DWORD LDR_APPL_RELOAD_PARAM;

// Flag indicating that update of application is (was) in progress.
// Flag can be used to detect error during loading of application.
extern BOOL LDR_APPL_UPDATE_B;

//LCD Contrast variable
extern DWORD LDR_DSP_SavedContrast;	


#ifdef __ARMCC_VERSION
#pragma arm section zidata
#endif

#endif
