
///////////////////////////////////////////////////////////////////////////
//
// Module   : PRSHW
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module PRSHW
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
// 030428  BW      Original version (based on EFTOS8 version 021004)
// 030513  BW      Replaced S_BAUD76800 with S_BAUD57600
// 030611  BW      Major change of handling of serial ports
// 040416  JB      Added RS2 serial port
// 040813  RZ      Updated for FIFO based communication driver support
//
///////////////////////////////////////////////////////////////////////////

#ifndef PRSHW_H
#define PRSHW_H

// Channel declarations
#define S_NO_CHANNELS   3       // Number of channels
#define RS_0            0       // SP0 in CPU
#define RS_1            1       // SP1 in CPU
#define RS_2            2       // SCC (Ext. UART)
#define SEC             RS_0    // Same as RS0
#define COM             RS_1    // Same as RS1
#define EXT             RS_2    // Same as RS2


// Signal definition
#define S_SIG_CD        0
#define S_SIG_CTS       1
#define S_SIG_DSR       2
#define S_SIG_DTR       3
#define S_SIG_RTS       4



// Interrupt type definition
#define S_INT_ALL       0
#define S_INT_RX        1
#define S_INT_TX        2
#define S_INT_ESI       3
#define S_INT_SRC       4

// Baudrate index definitions
#define S_BAUD300    0
#define S_BAUD600    1
#define S_BAUD1200   2
#define S_BAUD2400   3
#define S_BAUD4800   4
#define S_BAUD9600   5
#define S_BAUD19200  6
#define S_BAUD38400  7
#define S_BAUD57600  8
#define S_BAUD115200 9
#define S_BAUD172800 10

// Define mask for all serial port interrupts
//#define MASK_SERIAL_INT (BIT_UTXD0|BIT_UTXD1|BIT_URXD0|BIT_URXD1|BIT_EINT2|BIT_UERR01)
#define MASK_SERIAL_INT (BIT_UTXD0|BIT_UTXD1|BIT_URXD0|BIT_URXD1|BIT_EINT2|BIT_UERR01)



// Define mask for serial interrupts RS0, RS1, RS2
//#define MASK_RS0_INT (BIT_UTXD0|BIT_URXD0|BIT_UERR01)
//#define MASK_RS1_INT (BIT_UTXD1|BIT_URXD1|BIT_UERR01)
#define MASK_RS0_INT (BIT_UTXD0|BIT_URXD0)
#define MASK_RS1_INT (BIT_UTXD1|BIT_URXD1)
#define MASK_RS2_INT (BIT_EINT2)

// Structure holding parameters for CH
typedef struct 
{
    BOOL    bOpen;      // TRUE=Open, FALSE=Closed
    BOOL    bRxError;   // TRUE=Receive error detected
    BOOL    bRxIntEn;   // TRUE=Rx Int Enabled
    BOOL    bRxStopped; // TRUE=Recieve stopped
    BOOL    bTxActive;  // TRUE=Active
    BOOL    bTxBusy;    // TRUE=Busy
    BOOL    bTxIntEn;   // TRUE=Tx Int Enabled
    DWORD   nBaudRate;  // Baud rate (S_BAUD300 - S_BAUD19200)
    DWORD   nParity;    // Parity: 0=None, 1=Odd, 2=Even
    DWORD   nWordLen;   // Word length 7 or 8
    DWORD   nStopBits;  // Stopbits: 1 or 2
    DWORD   nFlow;      // See flow flags in Ch.h header file
    DWORD   nRxStart;   // Start pos in Receive ring buffer
    DWORD   nRxEnd;     // End pos in Receive ring buffer
    LPBYTE  pRxBuf;     // Receive ring buffer
    DWORD   nTxStart;   // Start pos in Transmit ring buffer
    DWORD   nTxEnd;     // End pos in Transmit ring buffer
    LPBYTE  pTxBuf;     // Transmit ring buffer
    DWORD   nTimer;     // Long timer
} CH_PARAM;

// Variable declarations

// Table of pointers to interrupt procedure tables
extern INT_PROC_TAB* S_IntPtr[S_NO_CHANNELS];


// Function declarations
DWORD S_GetBaudDiv(DWORD nCh, DWORD nBaudIndex);
BOOL  S_GetSig(DWORD nCh, DWORD nSig);
DWORD S_InitSerPort(DWORD nCh, CH_PARAM* pChParam);
void  S_IntReset(DWORD nCh, DWORD nIntType);
//void  S_OpenHW(void);
void  S_SetSig(DWORD nCh, DWORD nSig, BOOL bState);
void  S_WR_Buf(DWORD nCh, LPBYTE pData, DWORD* pOffset, DWORD nLen, DWORD nMask);
void  S_WR_Int(DWORD nCh);
DWORD S_TxStatus(DWORD nCh);
void  S_TxEnd(DWORD nCh);
void  S_RD_RX_FIFO(DWORD nCh, LPBYTE pData, LPBYTE pErr, DWORD* pOffset, DWORD nMask);
void  S_Set_AFC(DWORD nCh);
void  S_Clr_AFC(DWORD nCh);
void  S_DisableRX(DWORD nCh);

#endif
