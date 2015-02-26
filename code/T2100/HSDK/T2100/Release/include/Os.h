
///////////////////////////////////////////////////////////////////////////
//
// Module   : OS
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module OS
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
// 020712  BW      Original version (based on EFTOS7 version 990114)
// 030409  BW      Adapted for ARM compiler
// 030811  BW      Added function O_RetSignalParam and change of SIG_STRUCT
// 
///////////////////////////////////////////////////////////////////////////

#ifndef OS_H
#define OS_H

// Define max number of events in wait list 
#define O_MAX_NO_WAIT_EVT   10

// WAIT Structure
typedef struct EVT_STRUCT {
   DWORD     nNoEvt;    // Number of events in wait list
   BOOL FAR* EvtPtrArr[O_MAX_NO_WAIT_EVT];  // Array of pointers to event
} EVT_STRUCT;

// SIGNAL Structure
typedef struct SIG_STRUCT {
   DWORD nJobNr;        // Job number to be signalled (in)
   DWORD nRetCode;      // Return code from OS (out)
   DWORD* pParam;       // Pointer to signal parameter(s) to called process (optional)
} SIG_STRUCT;


// ESTABLISH Structure
typedef struct EST_STRUCT {
   PROCPTR  pStartProc; // Address to start procedure after establish (in)
   LPBYTE   pStack;     // Address to process (job) stack (in)
   DWORD    nStackSize; // Process stack size (in)   
   DWORD    nRetCode;   // Return code from OS (out)
   DWORD    nJobNr;     // Job number of established job (out) 
   BOOL FAR* pJobEvt;   // Pointer to job event flag (out)
} EST_STRUCT;


// CLOSE Structure
typedef struct CLOSE_STRUCT {
   DWORD    nJobNr;     // Job number of job to close (in) 
   DWORD    nRetCode;   // Return code from OS (out)
} CLOSE_STRUCT;


// Operating System functions
BOOL O_Close(CLOSE_STRUCT FAR* pCloseParam);
BOOL O_Establish(EST_STRUCT FAR* pEstParam);
void O_Open(void);
DWORD O_RetJobNrCurr(void);
DWORD* O_RetSignalParam(DWORD nJobNr);
BOOL O_Signal(SIG_STRUCT FAR* pSigParam);
void O_Terminate(void);
DWORD O_Wait(EVT_STRUCT FAR* pWaitList);
void O_WaitForResource(void);

#endif
