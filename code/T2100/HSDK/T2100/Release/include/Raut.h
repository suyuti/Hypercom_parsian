
///////////////////////////////////////////////////////////////////////////
//
// Module   : RAUT
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module RAUT
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
// 030417  BW      Original version (based on EFTOS8 version 030415)
// 030711  BW      Removed RAUT_Buf and most of R_Version indexes
// 050112  RZ      Changed OS version & revision to 9.16
// 050120  RZ      Changed OS version & revision to 9.17
// 050210  RZ      Changed OS version & revision to 9.18
// 050314  RZ      Changed OS version & revision to 9.19
// 050404  RZ      Changed OS version & revision to 9.20
// 050408  RZ      Changed OS version & revision to 9.21
// 050504  RZ      Changed OS version & revision to 9.22
// 050610  RZ      Changed OS version & revision to 9.23
// 050628  RZ      Changed OS version & revision to 9.24
// 050825  RZ      Changed OS version & revision to 9.25
// 050919  RZ      Changed OS version & revision to 9.26
// 051012  RZ      Changed OS version & revision to 9.27
// 051027  RZ      Changed OS version & revision to 9.28
// 060118  RZ      Changed OS version & revision to 9.29
// 060130  RZ      Changed OS version & revision to 9.30
// 060221  RZ      Changed OS version & revision to 9.31
// 060612  RZ      Changed OS version & revision to 9.32
// 060626  RZ      Changed OS version & revision to 9.33
// 060703  RZ      Changed OS version & revision to 9.34
// 060921  RZ      Changed OS version & revision to 9.35
// 070109  JC      Changed OS Version & revision to 9.36 
///////////////////////////////////////////////////////////////////////////

#ifndef RAUT_H
#define RAUT_H

extern DWORD nR_OpConnected;

// Definition of hardware unit indexes in R_Version
#define R_VER_ProcBoard     0

// Hardware versions used in R_Version
#define R_HW_UNDEFINED      0xff
#define R_HW_EFTOS9         0x09
#define R_SW_VERSION		0x03
#define R_SW_REVISION		0x06

// Structure used by array holding version data specific for each
// hardware unit in the terminal.
typedef struct {
    BYTE nVerHW;
    BYTE nVerSW;
    BYTE nRevSW;
} R_STATUS_DATA;
                         
// Define maximum number of hardware units
#define MAX_HWUNIT      1
                       
extern R_STATUS_DATA R_Version[MAX_HWUNIT];

//void RAUT(void);
void RAUT_Appl(void);

#endif
