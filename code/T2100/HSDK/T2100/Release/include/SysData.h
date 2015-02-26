
///////////////////////////////////////////////////////////////////////////
//
// Module   : SYS_DATA
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module SYS_DATA
//
//         Copyright (C) 2002 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 020712  BW      Original version
//
///////////////////////////////////////////////////////////////////////////

#ifndef SYSDATA_H
#define SYSDATA_H

#define NO_SYS_VEC_ENTRY        (32)
#define SYS_VEC_ALWAYS_PTR      0
#define SYS_VEC_BIG_PTR         1
#define SYS_VEC_SMALL_PTR       2
#define SYS_VEC_ALWAYS1_PTR     3
#define SYS_VEC_BIG1_PTR        4
#define SYS_VEC_SMALL1_PTR      5
#define SYS_VEC_APPL_PTR        6
#define SYS_VEC_KPTR            7
#define SYS_VEC_CHAR_GEN_PTR    8
#define SYS_VEC_TEST_PTR        9
#define SYS_VEC_OCP_KPTR       10
#define SYS_VEC_OCP_CHAR_PTR   11
#define SYS_VEC_MH_PTR         12
#define SYS_VEC_OVERFLOW_PTR   13
#define SYS_VEC_ZERO_DIV_PTR   14

#define NO_SYS_PAR_ENTRY        (16)
#define SYS_PAR_TIME_BEEP       0
#define SYS_PAR_TIME_BUZZ_ON    1
#define SYS_PAR_TIME_BUZZ_OFF   2
#define SYS_PAR_OP_TIME_BEEP    3
#define SYS_PAR_OP_BUZZ_ON      4
#define SYS_PAR_OP_BUZZ_OFF     5
#define SYS_PAR_LOADER          6
#define SYS_PAR_DEC_POINT       7

// Should be max. 16
#define SYS_VERSION_MAX         16

extern const LPVOID SYS_VecTable[NO_SYS_VEC_ENTRY];
extern const DWORD  SYS_ParTable[NO_SYS_PAR_ENTRY];
extern const BYTE   SYS_Version[SYS_VERSION_MAX+2];

#endif
