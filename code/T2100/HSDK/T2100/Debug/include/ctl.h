/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file ctl.h
//!     \brief defintions for CTL Device interface
//!     \since 2002
//!

#ifndef MAKE_VIKING

#ifndef CTL_H
#define CTL_H

/*
=============================================================================
*       Defintions for CTL Device interface                                 *
=============================================================================
*/


typedef struct {
	UBYTE CTLGTC_TIMOUT;	///< Desired Time Out value (0.5s unit)
	UBYTE CTLGTC_NUMCRD;	///< Number of Card present in field (Max 4)
	UBYTE CTLGTC_SERNM1[4];	///< Card 1 Serial Number
	UBYTE CTLGTC_CRDTP1;	///< Card 1 Type
	UBYTE CTLGTC_SERNM2[4];	///< Card 2 Serial Number
	UBYTE CTLGTC_CRDTP2;	///< Card 2 Type
	UBYTE CTLGTC_SERNM3[4];	///< Card 3 Serial Number
	UBYTE CTLGTC_CRDTP3;	///< Card 3 Type
	UBYTE CTLGTC_SERNM4[4];	///< Card 4 Serial Number
	UBYTE CTLGTC_CRDTP4;	///< Card 4 Type
} CTL_GETCARD_RECORD;


typedef struct {
	UBYTE CTLOPC_KEYSET;	///< Desired Key Set
	UBYTE CTLOPC_KEYMOD;	///< Desired Key Mode
	UBYTE CTLOPC_SECNUM;	///< Desired Sector Number
	UBYTE CTLOPC_SERNUM[4];	///< Desired Card's Serial Number
	UBYTE CTLOPC_DICODE;	///< Desired Response Diagnostic Code
} CTL_OPENCARD_RECORD;


typedef struct {
	UBYTE CTLKEY_KEYSET;	///< Desired Key Set
	UBYTE CTLKEY_KEYMOD;	///< Desired Key Mode
	UBYTE CTLKEY_SECNUM;	///< Desired Sector Number
	UBYTE CTLKEY_NEWKEY[6];///< Desired New Key
} CTL_LOADKEY_RECORD; 


// ===========================================================================
//
// WRITE Command Equates

#define CTL_INCBLK	0x01    // Increment Block
#define CTL_DECBLK	0x02    // Decrement Block
#define CTL_WRBLK	0x03    // Write Block
#define CTL_RDBLK	0x04    // Read Block

// ------------------------------
//
///       Define the CTL Driver's Increment Block Record Structure

typedef struct {
	UBYTE CTLINC_FUNTYP;			///< Desired Function Type
	UBYTE CTLINC_BLKNUM;			///< Desired Block Number
	unsigned long CTLINC_AMOUNT;	///< Desired Amount (binary format)
	unsigned long CTLINC_BALNCE;	///< Returned Balance from CTL Driver
} CTL_WRITE_INC;

typedef struct {
	UBYTE	CTLDEC_FUNTYP;			///< Desired Function Type
	UBYTE	CTLDEC_BLKNUM;			///< Desired Block Number
	unsigned long CTLDEC_AMOUNT;	///< Desired Amount (binary format)
	unsigned long CTLDEC_BALNCE;	///< Returned Balance from CTL Driver
} CTL_WRITE_DEC;

// ------------------------------
//
///       Define the CTL Driver's Write Block Record Structure


typedef struct {
	UBYTE CTLWRB_FUNTYP;			///< Desired Function Type
	UBYTE CTLWRB_BLKNUM;			///< Desired Block Number
	UBYTE CTLWRB_BYTOFF;			///< Desired Byte Offset
	UBYTE CTLWRB_NUMBYT;			///< Desired Number of Bytes
	UBYTE CTLWRB_DATA[16];			///< Desired Data Block
} CTL_WRITE_BLOCK;

typedef struct {
	UBYTE CTLRDB_FUNTYP;			///< Desired Function Type
	UBYTE CTLRDB_BLKNUM;			///< Desired Block Number
	UBYTE CTLRDB_BYTOFF;			///< Desired Byte Offset
	UBYTE CTLRDB_NUMBYT;			///< Desired Number of Bytes
	UBYTE CTLRDB_DATA[16];			///< Desired Data Block
} CTL_READ_BLOCK;

// -------------------------------------------------------------
// Enumerated types for CTL_GETCARD Card Type :
//

#define CTL_OTH_TYP 0x00    ; Other Card Type
#define CTL_MIF_STD 0x01    ; Mifare Standard Card Type
#define CTL_MIF_LGT 0x02    ; Mifare Light Card Type

// -------------------------------------------------------------
// Types for CTL_OPENCARD Key Set, Key Mode, Sector Number :
//
#define CTL_KEYSET0		0x00    // Key Set 0
#define CTL_KEYSET1		0x01    // Key Set 1
#define CTL_KEYSET2		0x02    // Key Set 2
#define CTL_KEYSET3		0x03    // Key Set 3

#define CTL_KEYMODE_A   0x00    // Key Mode A
#define CTL_KEYMODE_B   0x01    // Key Mode B

#define CTL_SECTOR0     0x00    // Sector Number 0
#define CTL_SECTOR1     0x01    // Sector Number 1
#define CTL_SECTOR2     0x02    // Sector Number 2
#define CTL_SECTOR3     0x03    // Sector Number 3
#define CTL_SECTOR4     0x04    // Sector Number 4
#define CTL_SECTOR5     0x05    // Sector Number 5
#define CTL_SECTOR6     0x06    // Sector Number 6
#define CTL_SECTOR7     0x07    // Sector Number 7
#define CTL_SECTOR8     0x08    // Sector Number 8
#define CTL_SECTOR9     0x09    // Sector Number 9
#define CTL_SECTOR10    0x0A    // Sector Number 10
#define CTL_SECTOR11    0x0B    // Sector Number 11
#define CTL_SECTOR12    0x0C    // Sector Number 12
#define CTL_SECTOR13    0x0D    // Sector Number 13
#define CTL_SECTOR14    0x0E    // Sector Number 14
#define CTL_SECTOR15    0x0F    // Sector Number 15


// ====================================================================
// BEGINNING OF "C" LANGUAGE DEFINITIONS -- C_CODE
// ====================================================================

// CONFIG Mode Equates (IOCSMODE)

                // Load Key to Contactless Device
#define CTL_LOADKEY     0x00
                // Get Card present in field
#define CTL_GETCARD     0x01
                // Cancel Get Card action
#define CTL_NOGETCARD   0x02
                // Open Card in field
#define CTL_OPENCARD    0x03
                // Close Card in field
#define CTL_CLOSECARD   0x04
                // Field On/Off Control
#define CTL_OPERCTRL    0x05

// ===========================================================================

// CONFIG Command Equates :

// -------------------------------------------------------------
// Enumerated types for CTL_OPERCTRL Config Command :

                // Turn Field Off
#define CTL_FIELD_OFF   0x0000
                // Turn Field On
#define CTL_FIELD_ON    0x0001

#define CTL_ANTENNA_A	0x0000    // Antenna A
#define CTL_ANTENNA_B	0x0100    // Antenna B

#endif // ifdef CTL_H

#endif // MAKE_VIKING
