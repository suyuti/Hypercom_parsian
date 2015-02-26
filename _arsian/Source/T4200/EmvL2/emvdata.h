/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 2001                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  EMV Level 2 Library in C                        *        */
/*  *                                                               *        */
/*  *  COMPONENT :  EMV Library - Global Data Definitions           *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emvdata.h                                       *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR       COMMENT                          *        */
/*  *  --------     ----------    ----------                        *        */
/*  *  23 NOV 01     Ian Smith     1st Draft                        *        */
/*  *  11 JAN 02   A.Shuttleworth  Moved CandTextType to emv_l2.h   *        */
/*  *  15 Jan 02   A.Shuttleworth  Added LibControl                 *        */
/*  *                                                               *        */
/*  *****************************************************************        */

#include "scard.h"

// smart card definitions
#define Cla    0
#define Ins    1
#define P1     2
#define P2     3
#define Lc     4
#define Cmd    5
// Le location is variable 



// SW1-SW2 Card response constants
#define EMVL2_9000     0x9000
#define EMVL2_6283     0x6283 
#define EMVL2_6300     0x6300
#define EMVL2_63Cx     0x63C0
#define EMVL2_6983     0x6983
#define EMVL2_6984     0x6984
#define EMVL2_6985     0x6985
#define EMVL2_6A81     0x6A81
#define EMVL2_6A82     0x6A82 
#define EMVL2_6A83     0x6A83
#define EMVL2_6A88     0x6A88
#define EMVL2_6D00     0x6D00
#define EMVL2_6E00     0x6E00

// Atmel response constants
#define EMVL2_6F0A     0x6F0A  // Issuer public key certificate revoked
#define EMVL2_6F0D     0x6F0D  // Decrypted and plain text CID do not match
#define EMVL2_6F0E     0x6F0E  // Anticipated remainder not present


// Candidate list data structure
typedef struct 
{
   UBYTE a_length;   // length of the AID returned by the card 
   UBYTE aid[16];    // AID returned by the card
   UBYTE api;        // application priority '0' - 'F' (top bit = confirmation required)
   UBYTE control;    // bitmap of TermMaster supplied stuff
   UBYTE adt_id;     // ID of corresponding Additional Data Tag record;  
} CandDataType;      // see EMV Terminal Spec, Annex A, page A-9 or ISO 8859



// Structure of DDF "stack"
typedef struct
{
   UBYTE d_length;
   UBYTE df_name[16];
} DDF_StoreType;

// smart card comms buffers
extern UBYTE  SC_CmdBytes[MaxSC];  // Smart card Command buffer
extern UWORD  SC_CmdLen;           // Length of smart card command
extern UBYTE  SC_RspBytes[MaxSC];  // response buffer
extern UWORD  SC_RspLen;           // response length
                                   // Sw1 Sw2 are stored in the debug_buffer
extern UBYTE  SC_CmdStatus;        // smart card status
extern UBYTE  SC_NAD; 

/* Buffer to hold records that are to be authenticated */
extern UBYTE common_buffer[EMVL2_MAX_CBUF]; // BIG !!! sized to hold 3 entire records
extern UWORD cbuf_index;                    // Index into auth_buffer 


#define EMVL2_DF_SIZE       10 // Size of DDF and ADF stores

// export the external "text" list
extern CandTextType CandTextList[EMVL2_DF_SIZE];
  
/* Used by the authentication process to use the SIM cryptoprocessor */
extern  SCRDDEVICE CryptLocn;

// Boolean to suppress/print signature line
extern Bool SuppressSigLine;

// Boolean to include online encrypted pin TAG DF78
extern Bool IncludeEncryptedPin ;

// Control options for the library from TMS
extern  UBYTE LibControl;

// Debug memory
extern  UBYTE debug_buffer[EMVL2_DEBUG_SIZE];
extern UBYTE EMV_KernelConfig;     // Operating mode of EMV Library


// CVM variables
extern UBYTE CurrentCVMState;
extern UBYTE TransportCVMList[252];


extern UBYTE CustTagTotal;

