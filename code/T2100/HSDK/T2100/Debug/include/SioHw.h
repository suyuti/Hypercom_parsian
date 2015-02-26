///////////////////////////////////////////////////////////////////////////
//
// Module   : SIOHW
// Part of  : SSW, EFTOS
// Created  : Roman Zaytsev
// Abstract : Header file of module SIOHW
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

#ifndef SIOHW_H
#define SIOHW_H

#include "SswDef.h"


// Clock source
#define SIO_INT_CLOCK		0x00
#define SIO_EXT_CLOCK		0x80

// Data direction
#define SIO_MSB_FIRST		0x00
#define SIO_LSB_FIRST		0x40

// Clock edge
#define SIO_FALLING_EDGE	0x00
#define SIO_RISING_EDGE		0x10

// SIO servicing type
#define SIO_POLLING			0x01

// SIO data register
#define SIODAT		(0x1d14004)

// Baudrate index definitions
#define SIO_BAUD_32K 	0
#define SIO_BAUD_64K 	1
#define SIO_BAUD_128K	2
#define SIO_BAUD_256K	3
#define SIO_BAUD_512K	4
#define SIO_BAUD_1M		5
#define SIO_BAUD_2M		6
#define SIO_BAUD_4M		7
#define SIO_BAUD_8M		8
#define SIO_BAUD_10M	9

// Define mask for SIO interrupt
#define MASK_SIO_INT	BIT_SIO


// Variable declarations
// Pointer to interrupt procedure
extern PROCPTR S_SioIntPtr;


// Function declarations
void S_InitSio(DWORD nBaudRate, DWORD nInterval, DWORD nClock, DWORD nDir, DWORD nEdge, BOOL bReady);
void S_DisableSio(BOOL bReady);
void S_ResetSio(void);
void S_SioTx(BYTE nData);
void S_SioRx(LPBYTE pData);

#endif
