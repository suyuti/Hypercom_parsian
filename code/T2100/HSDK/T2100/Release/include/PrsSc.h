
///////////////////////////////////////////////////////////////////////////
//
// Module   : PRSSC
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module PRSSC
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
// 020712  BW      Original version (based on EFTOS7 version 990303)
// 030611  BW      Major change of handling of serial ports
//
///////////////////////////////////////////////////////////////////////////

#ifndef PRSSC_H
#define PRSSC_H

#include "PrsHw.h"


// Command definitions
#define S_READ_CMD            1     // Read command
#define S_WRITE_CMD           2     // Write command
#define S_TERMINATE_CMD       4     // Terminate command
#define S_CANCEL_PROT_CMD     8     // Cancel command
#define S_CANCEL_IN_BG_CMD    16    // Cancel in background command
#define S_PROCESS_CMD         32    // Process command (processing of protocol)
 

// Structure holding data transferred to and from application
typedef struct {
    DWORD   nCmd;       // Command  
    BOOL    bBusy;      // Busy mark (of command)
    LPBYTE  pAD;        // Start address to buffer
    DWORD   nCnt;       // Buffer size/counter
    BOOL    bEvt;       // Event flag to indicate command ready
    DWORD   nRetStat;   // Return status
} S_CH_DATA;

extern S_CH_DATA volatile S_ChRdData[S_NO_CHANNELS];
extern S_CH_DATA volatile S_ChWrData[S_NO_CHANNELS];

typedef void (FAR_FUNC *TWO_PARAM_PROCPTR)(DWORD, DWORD);

// Structure holding data for each specific protocol
typedef struct {
    LPBYTE  pAD;        // Start address to buffer (not used?)
    DWORD   nCnt;       // Buffer size/counter (not used?)
    DWORD   nBaud;      // Baud rate (0=300,1=600,...,5=9600)
    DWORD   nRetCode;   // Return code
    S_CH_DATA* pWQ;     // Pointer to work (job) queue
    BOOL    bStarted;   // Indicates if protocol is started
    PARAM_PROCPTR pCncProc; // Pointer to cancel procedure
    DWORD   nWrRetry;   // Number of write retry
    LPBYTE  pParam;     // Pointer to parameters
    TWO_PARAM_PROCPTR pEntryProc; // Entry point to protocol handler
    BOOL    bScanChWr;  // Indicates area of CH to scan next (TRUE=Write area)
    BOOL    bOpened;    // Indicates if protocol is opened
} S_PR_DATA;
extern S_PR_DATA S_ProtData[S_NO_CHANNELS];


// Protocol Schedular functions
void S_CancelClear(DWORD nCh);
void S_CancelCh(DWORD nCh, DWORD nCmd);
void S_CancelTerm(DWORD nCh);
void S_ClearProt(DWORD nCh);
void S_Disconn(DWORD nCh, DWORD nRetCode);
void S_Open(void);
DWORD S_RetWrRetry(DWORD nCh);
void S_Scan(void);

#endif
