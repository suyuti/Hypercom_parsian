///////////////////////////////////////////////////////////////////////////
//
// Module   : BCI
// Part of  : SSW, EFTOS
// Created  : Roman Zaytsev
// Abstract : Header file of module BCI
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
// 041011  RZ      Original version
//
///////////////////////////////////////////////////////////////////////////

#ifndef BCI_H
#define BCI_H

// Extend driver facilities
//#define BCI_EXT

// Define battery parameters
#define BAT_VOLTAGE		7400	// mV
#define BAT_CAPACITY	2200	// mAh
#define BAT_HW_RS		33		// Om
#define BAT_HW_RB1		330		// kOm
#define BAT_HW_RB2		47		// kOm
#define BAT_NMCV		0xE0	// (BYTE)(~((256 * BAT_VOLTAGE * BAT_HW_RB2) / (7400 * (BAT_HW_RB1 + BAT_HW_RB2))) + 1)

// Define registers access mode
#define BCI_REG_RO	0x00	// Read only
#define BCI_REG_RW	0x80	// Read/Write
// Define error notification
#define BCI_REG_ERR	0x40	// Error flag

// Define bq2050H Command & Status Registers
#define	BCI_ADDR_FLGS1	(0x01 | BCI_REG_RO)	// Primary status flags register
#define	BCI_ADDR_TMP	(0x02 | BCI_REG_RO)	// Temperature register
#define	BCI_ADDR_NACH	(0x03 | BCI_REG_RW)	// Nominal available capacity high byte register
#define	BCI_ADDR_NACL	(0x17 | BCI_REG_RW)	// Nominal available capacity low byte register
#define	BCI_ADDR_BATID	(0x04 | BCI_REG_RW)	// Battery identification register
#define	BCI_ADDR_LMD	(0x05 | BCI_REG_RW)	// Last measured discharge register
#define	BCI_ADDR_FLGS2	(0x06 | BCI_REG_RO)	// Secondary status flags register
#define	BCI_ADDR_PPD	(0x07 | BCI_REG_RO)	// Program pin pull-down register
#define	BCI_ADDR_PPU	(0x08 | BCI_REG_RO)	// Program pin pull-up register
#define	BCI_ADDR_CPI	(0x09 | BCI_REG_RW)	// Capacity inaccurate count register
#define	BCI_ADDR_VSB	(0x0B | BCI_REG_RO)	// Battery voltage register
#define	BCI_ADDR_VTS	(0x0C | BCI_REG_RW)	// End-of-discarge threshold select register
#define	BCI_ADDR_CACT	(0x0D | BCI_REG_RW)	// Temperature and Discharge Rate compensated available capacity
#define	BCI_ADDR_CACD	(0x0E | BCI_REG_RW)	// Discharge Rate compensated available capacity
#define	BCI_ADDR_SAEH	(0x0F | BCI_REG_RO)	// Scaled available energy high byte register
#define	BCI_ADDR_SAEL	(0x10 | BCI_REG_RO)	// Scaled available energy low byte register
#define	BCI_ADDR_RCAC	(0x11 | BCI_REG_RO)	// Relative CAC
#define	BCI_ADDR_VSRH	(0x12 | BCI_REG_RO)	// Current scale high
#define	BCI_ADDR_VSRL	(0x13 | BCI_REG_RO)	// Current scale low
#define	BCI_ADDR_NMCV	(0x15 | BCI_REG_RW)	// Maximum cell voltage
#define	BCI_ADDR_DCR	(0x18 | BCI_REG_RW)	// Discharge register
#define	BCI_ADDR_PPFC	(0x1E | BCI_REG_RW)	// Program pin data
#define	BCI_ADDR_INTSS	(0x38 | BCI_REG_RO)	// Vos Interrupt
#define	BCI_ADDR_RST	(0x39 | BCI_REG_RW)	// Reset register
#define	BCI_ADDR_HEXFF	(0x3F | BCI_REG_RW)	// Check register

// Structure holding the batary state
typedef struct 
{
    BOOL	bValid;	// Data structure valid flag
    BOOL	bAlarm;	// Battery charge alarm level
    DWORD   nC;		// Battery available capacity
    DWORD   nV;		// Battery voltage
	int		nT;		// Battery temperature
	DWORD	nNAC;	// Nominal available capacity
	DWORD	nStat;	// Status bits
} BCI_INFO_DATA;

// Public functions available for application
DWORD BCI_Open (DWORD nTimeout, BOOL bEnhance);
void  BCI_Close(void);
BOOL  BCI_Check(void);
BOOL  BCI_FullStatus (BCI_INFO_DATA* pStatus);
DWORD BCI_Capacity(void);
BOOL  BCI_BatteryAlarm(void);
BOOL  BCI_BatteryCharge(void);
#ifdef	BCI_EXT
void BCI_SetRegister(BYTE nAddr, BYTE nData);
void BCI_GetRegister(BYTE nAddr);
#endif	// BCI_EXT

#endif

