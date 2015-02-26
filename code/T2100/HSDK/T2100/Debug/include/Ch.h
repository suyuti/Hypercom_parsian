
///////////////////////////////////////////////////////////////////////////
//
// Module   : CH
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module CH
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
// 020712  BW      Original version (based on EFTOS7 version 020206)
// 030617  BW      Major change of handling of serial ports.
//                 One module common for all files handle all channels.
//                 Several functions made public. Dedicated timers.
// 031013  BW      New option to prevent use of DTR signal
// 040419  JB      Added support for RS0
// 040813  RZ      Updated for FIFO based communication driver support
// 040819  RZ      Added CH_Write & CH_Read functions
//
///////////////////////////////////////////////////////////////////////////

#ifndef CH_H
#define CH_H

#include "PrsHw.h"

#define CH_PARITY_NONE  0
#define CH_PARITY_ODD   1
#define CH_PARITY_EVEN  2

#define CH_CHECKDSR     0x01    // Flow flags
#define CH_CHECKCTS     0x02
#define CH_USERTS       0x04
#define CH_CTS_H_BUSY   0x08    // CTS = high means they are busy
#define CH_RTS_H_BUSY   0x10    // RTS = high means I am busy
#define CH_NO_USE_DTR   0x20    // Set this bit to avoid signalling of DTR
#define CH_NO_USE_AFC	0x40	// Set this bit to avoid Automatic Flow Control usage

// Hardware error codes
#define CH_OVERRUN_ERROR	0x01000000
#define CH_PARITY_ERROR		0x02000000
#define CH_FRAME_ERROR		0x04000000
#define CH_BREAK_DETECT		0x08000000


typedef struct
{
    DWORD   nBaudRate;  // Baudrate (S_BAUD300 - S_BAUD115200 from PRSHW.H)
    DWORD   nParity;    // Parity: 0=None, 1=Odd, 2=Even
    DWORD   nWordLen;   // Word length 7 or 8
    DWORD   nStopBits;  // Stopbits: 1 or 2
    DWORD   nFlow;      // See flow flags above
} CH_STRUCT;




// Public functions available for application
void  CH_Clear(DWORD nCh);
void  CH_Close(DWORD nCh);
DWORD CH_Get(DWORD nCh, LPBYTE pChar);
DWORD CH_IsRxChar(DWORD nCh);
DWORD CH_IsTxChar(DWORD nCh);
DWORD CH_Open(DWORD nCh, CH_STRUCT* pParam);
DWORD CH_Put(DWORD nCh, BYTE nChar);
DWORD CH_Write(DWORD nCh, LPBYTE pData, DWORD* pLen);
DWORD CH_Read(DWORD nCh, LPBYTE pData, DWORD* pLen);


// Functions used internally by system software (protocol drivers)
void CH_ClearRx(DWORD nCh);
void CH_ClearTx(DWORD nCh);
BOOL CH_IsOperable(DWORD nCh);
BOOL CH_IsReady(DWORD nCh);
BOOL CH_IsRxError(DWORD nCh);
void CH_SetBusy(DWORD nCh);
void CH_SetReady(DWORD nCh);

#endif
