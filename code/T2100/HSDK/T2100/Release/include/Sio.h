///////////////////////////////////////////////////////////////////////////
//
// Module   : SIO
// Part of  : SSW, EFTOS
// Created  : Roman Zaytsev
// Abstract : Header file of module SIO
//
//         Copyright (C) 2004 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 040916  RZ      Original version
//
///////////////////////////////////////////////////////////////////////////

#ifndef SIO_H
#define SIO_H

#include "SioHw.h"

typedef struct
{
    BOOL	bReady;		// Use ready pin
    DWORD   nBaudRate;  // Baudrate
	DWORD	nInterval;	// interval between 8-bits data
    DWORD   nClock;	    // Clock source (internal or external)
    DWORD   nDirection;	// Data direction (MSB or LSB)
    DWORD   nEdge;      // Clock edge (falling or rising)
} SIO_STRUCT;

// Structure holding the driver state
typedef struct 
{
    BOOL    bEvt;       // Event flag to indicate when command is ready
    DWORD   nRetStat;   // Return status
} SIO_MOD_DATA;

extern SIO_MOD_DATA volatile SIO_Data;

// Public functions available for application
DWORD SIO_Open(const SIO_STRUCT* pParam);
void  SIO_Close(void);
void  SIO_Cancel(void);
void  SIO_Resume(void);
DWORD SIO_TxRx(LPBYTE pTx, DWORD nTxLen, LPBYTE pRx, DWORD nRxLen);

#endif
