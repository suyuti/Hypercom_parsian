

/*                                                                           */
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
/*                                                                           */


#include "basictyp.h"
#include "scard.h"

#include "emv_l2.h"

#ifdef _TARG_Z80
#pragma region("ram=emvdata")
#endif

// The library operating mode
UBYTE emv_mode;   // EMVL2_311 | EMVL2_4

// smart card comms buffers
UBYTE  SC_CmdBytes[MaxSC];  // Smart card Command buffer
UWORD  SC_CmdLen;           // Length of smart card command
UBYTE  SC_RspBytes[MaxSC];  // response buffer
UWORD  SC_RspLen;           // response length
                            // Sw1 Sw2 are stored in the debug_buffer
UBYTE  SC_CmdStatus;        // smart card status
UBYTE  SC_NAD; 


/* Common buffer */
UBYTE common_buffer[EMVL2_MAX_CBUF]; // BIG !!! 
UWORD cbuf_index;                  // Index into auth_data 

// Create the external candidate "text" list
CandTextType CandTextList[EMVL2_DF_SIZE];

/* Used by the authentication process to use the SIM cryptoprocessor */
SCRDDEVICE CryptLocn;

// Boolean to suppress/print signature line
Bool SuppressSigLine;

// Control options for the library from TMS
UBYTE LibControl;

// Debug memory
UBYTE debug_buffer[EMVL2_DEBUG_SIZE];

// CVM state
UBYTE CurrentCVMState;
UBYTE TransportCVMList[252];


// number of custom defined tags
UBYTE CustTagTotal;
  
#ifdef _TARG_Z80
#pragma region("ram=ram")
#endif

