/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 2001                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  EMV Level 2 Library in C                        *        */
/*  *                                                               *        */
/*  *  COMPONENT :  Additional Tag Store Data                       *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emv_atdata.c                                    *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR       COMMENT                          *        */
/*  *  --------     ----------    ----------                        *        */
/*  *                                                               *        */
/*  *****************************************************************        */

// This file contains buffer declarations which may require modification
// when compiling for non Z80 targets.

#include "basictyp.h"

// System header file
#include  "emv_l2.h"

// Own header
#include  "emv_at.h"

#ifdef __GNUC__
#define __init__(A) A
#else
#define __init__(A)
#ifndef __attribute__
#define __attribute__(A)
#endif
#endif

// ---------- insert target specific pragmas here --------------------------
#ifdef _TARG_Z80
#pragma region("ram=adtdata")
#elif defined( __ARMCC_VERSION )
#pragma arm section zidata="adtdata"
#endif

// 1. Persistant buffers (must persist after configuration download)

// This buffer contains the descriptor data for the additional custom tags
//! Additional Tags Table formatter
__attribute__((section ("adtdata")))
UBYTE ADTFMT __init__(=0);
//! Additional Tags Table
// Format + 128 + LRC = 130 bytes x 10 = 1300 bytes
__attribute__((section ("adtdata")))
ADTRecType ADTData[EMVL2_ADT_MAX] __init__(={{0}});
//! Additional Tags Table LRC
__attribute__((section ("adtdata")))
UBYTE ADTLRC __init__(=0);


// 2. Dynamic buffers

//! This buffer contains the custom tag store and is re-populated by the terminal
//! during each transaction. When compiling for non Z80 targets, this buffer must be 
//! aligned on an even byte boundary.
__attribute__((section ("adtdata")))
UBYTE CustTagStore[EMVL2_ADT_SIZE] __init__(={0});

#ifdef _TARG_Z80
#pragma region("ram=ram")
#endif
// ---------- insert target specific pragmas here --------------------------

