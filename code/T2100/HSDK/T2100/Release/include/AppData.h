
///////////////////////////////////////////////////////////////////////////
//
// Module   : APPDATA
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module APPDATA
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
// 040629  ASA     Changed for GNU C support
//
///////////////////////////////////////////////////////////////////////////

#ifndef APPDATA_H
#define APPDATA_H


#ifndef BASIC_TYPE_DEFINED
typedef unsigned char BYTE;
typedef unsigned long DWORD;
#endif


// Define size of check area in RAM written with pattern
// to detect if RAM has lost power (this value must be the same
// as CHECK_AREA_SIZE in LdrData.h)
#define CHECK_AREA_SIZE     16


// Check area in RAM written with pattern
#ifdef __ARMCC_VERSION
#pragma arm section zidata="AppData"
#endif
extern BYTE APP_CheckArea[CHECK_AREA_SIZE];
#ifdef __ARMCC_VERSION
#pragma arm section zidata
#endif

#endif
