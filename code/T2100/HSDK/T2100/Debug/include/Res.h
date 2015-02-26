
///////////////////////////////////////////////////////////////////////////
//
// Module   : RES
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module RES
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
// 020712  BW      Original version
// 030613  BW      Variable nR_LanAddr removed
// 030909  BW      Variable nR_FileName removed
// 050112  RZ      Deleted R_Delay function
//
///////////////////////////////////////////////////////////////////////////

#ifndef RES_H
#define RES_H

// TRUE if reload of application is requested.                         
extern BOOL nR_ReloadReq;

// Hardware type autodetect value
// Contain values from "SswDef.h" file 
extern DWORD HARDWARE_TYPE_DETECT;

void R_Init(void);
BOOL R_IsBigRestartReq(void);
BOOL R_IsColdRestart(void);
void R_RebootReq(void);
void R_ReloadReq(DWORD LoadSrc);
void R_VectorJmp(DWORD nEntry);
void R_TestStart(void);
void R_LdrStart(void);

#endif
