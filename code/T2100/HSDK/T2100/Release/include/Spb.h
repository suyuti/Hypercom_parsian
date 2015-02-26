///////////////////////////////////////////////////////////////////////////
//
// Module   : SPBSE
// Part of  : SSW, EFTOS
// Created  : Roman Zaytsev
// Abstract : Header file of module SPBSE
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
// 041127  RZ      Original version
//
///////////////////////////////////////////////////////////////////////////

#ifndef SPB_H
#define SPB_H


// Definition of slave numbers in SPB (logical addresses)
#define SPB_SLAVE           0   // Reserved - not used
#define SPB_SLAVE_SECCHIP   1   // Reserved - not used
#define SPB_SLAVE_SCR       2
#define SPB_SLAVE_PRINTER   3   // Reserved - not used
#define SPB_SLAVE_TEST      4   // Used for test purposes
#define SPB_SLAVE_KBD       5   // Used for Keyboard and Buzzer
#define SPB_SLAVE_OP        6   // Reserved - not used
#define SPB_SLAVE_DATAFLASH 7   // Reserved - not used
#define SPB_SLAVE_SAM       9


// Structure used by array holding data specific for each slave
typedef struct 
{
	DWORD		nCmd;		// Command  
	LPBYTE		pAD;		// Address (pointer) to application data buffer
	DWORD		nCnt;		// Buffer size
	BOOL		bEvt;		// Event flag to indicate when command is ready
	DWORD		nRetStat;	// Return status
} SPB_MOD_DATA_S;

// Keyboard event flag (set after new key pressed)
extern volatile BOOL bSPB_KeyboardEvt;

// Multi Processor Bus functions
void SPB_Cancel(DWORD nSlaveNr);
BOOL SPB_Read(DWORD nSlaveNr, LPBYTE pBuffer, DWORD nNoBytes);
SPB_MOD_DATA_S* SPB_RetModDataPtr(DWORD nSlaveNr);
BOOL SPB_Write(DWORD nSlaveNr, LPBYTE pBuffer, DWORD nNoBytes);


// Functions used internally by EFTOS
void SPB_Open(void);
BOOL SPB_ReadOnce(DWORD nSlaveNr, LPBYTE pBuffer, DWORD nNoBytes);

#endif
