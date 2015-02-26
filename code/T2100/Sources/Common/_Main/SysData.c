
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             SysData.c
//        EFTOS Try Application
//
//=============================================================================

// HSDK includes
#include "eftos.h"
#include "SysData.h"
#include "kbddev.inc"


extern void __rt_entry( void );

// Normal key board table
CONSTBYTE KeyTab[] = {
    KUP, KSELECT,  KDOWN,
    K1,  K2,  K3,  KCANCEL,
    K4,  K5,  K6,  KCLEAR,
    K7,  K8,  K9,  KENTER,
         K0 };
 
LPVOID const SYS_VecTable[ NO_SYS_VEC_ENTRY ] = {
    NULL,               // ALWAYS_PTR
    NULL,               // BIG_PTR
    NULL,               // SMALL_PTR
    NULL,               // ALWAYS1_PTR
    NULL,               // BIG1_PTR
    NULL,               // SMALL1_PTR
    (LPVOID)__rt_entry, // APPL_PTR 
    (LPVOID)KeyTab,     // KPTR
    NULL,               // CHAR_GEN_PTR
    NULL,               // TEST_PTR
    (LPVOID)KeyTab,     // OCP_KPTR
    NULL,               // OCP_CHAR_PTR
    NULL,               // MH_PTR
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL,               // not used
    NULL                // not used
};

DWORD const SYS_ParTable[NO_SYS_PAR_ENTRY] = {
    4,                  // TIME_BEEP
    5,                  // TIME_BUZZ_ON
    20,                 // TIME_BUZZ_OFF
    4,                  // OP_TIME_BEEP
    5,                  // OP_TIME_BUZZ_ON
    20,                 // OP_TIME_BUZZ_OFF
    0,                  // LOADER=1
    0,                  // not used
    0,                  // not used
    0,                  // not used
    0,                  // not used
    0,                  // not used
    0,                  // not used
    0,                  // not used
    0,                  // not used
    0                   // not used
};

const BYTE SYS_Version[SYS_VERSION_MAX+2]=
    {16,' ',' ',' ',' ','F','U','x','1','9','0','1','0','T','1',' ',' ',0};

