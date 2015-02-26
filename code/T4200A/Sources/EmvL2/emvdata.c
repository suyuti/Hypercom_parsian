/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 2001                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  EMV Level 2 Library in C                        *        */
/*  *                                                               *        */
/*  *  COMPONENT :  EMV Library - Global Data                       *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emvdata.c                                       *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR       COMMENT                          *        */
/*  *  --------     ----------    ----------                        *        */
/*  *  23 NOV 01     Ian Smith     1st Draft                        *        */
/*  *  15 Jan 02   A.Shuttleworth  Added LibControl                 *        */
/*  *                                                               *        */
/*  *****************************************************************        */
#include "basictyp.h"
#include "scard.h"

#include "emv_l2.h"

#ifdef _TARG_Z80
#pragma region("ram=emvdata")
#endif

//! The library operating mode (EMVL2_v311/EMVL2_v4)
UBYTE emv_mode;

//! smart card comms buffers

//! Smart card Command buffer
UBYTE  SC_CmdBytes[MaxSC];
//! Length of smart card command
UWORD  SC_CmdLen;
//! Response buffer
UBYTE  SC_RspBytes[MaxSC];
//! Response length (Sw1 Sw2 are stored in the debug_buffer)
UWORD  SC_RspLen;
// Smart card status
UBYTE  SC_CmdStatus;
UBYTE  SC_NAD; 


//! Common buffer
UBYTE common_buffer[EMVL2_MAX_CBUF]; // BIG !!! 
//! Index into auth_data 
UWORD cbuf_index;

//! Create the external candidate "text" list
CandTextType CandTextList[EMVL2_DF_SIZE];

//! Used by the authentication process to use the SIM cryptoprocessor
SCRDDEVICE CryptLocn;

//! Boolean to suppress/print signature line
Bool SuppressSigLine;

// Boolean to include online encrypted pin TAG DF78
Bool IncludeEncryptedPin ;

// Control options for the library from TMS
UBYTE LibControl;

//! Debug memory
UBYTE debug_buffer[EMVL2_DEBUG_SIZE];

//! CVM state
UBYTE CurrentCVMState;
UBYTE TransportCVMList[252];

UBYTE EMV_KernelConfig;     // Operating mode of EMV Library


//! Number of custom defined tags
UBYTE CustTagTotal;
  
#ifdef _TARG_Z80
#pragma region("ram=ram")
#endif

