
///////////////////////////////////////////////////////////////////////////
//
// Module   : G1S
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module G1S
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
// 020712  BW      Original version (based on EFTOS7 version 020205)
// 030611  BW      Major change of handling of serial ports.
//                 One module common for all files handle all channels.
// 031017  JB	   Added mode 4 for parameter nFlow (Not use DTR)
//
///////////////////////////////////////////////////////////////////////////

#ifndef G1S_H
#define G1S_H

// Structure holding parameters input to protocol from application
typedef struct {
    BOOL    b8Bit;      // Use 8 data bit if set (otherwise 7 data bit)
    BOOL    bETX_InLRC; // Include ETX in LRC
    BOOL    bParity;    // Use Parity
    BOOL    bParityEven;// Even Parity
    BOOL    bLRC;       // Use LRC (checksum)
    DWORD   nFlow;      // 0=no flow control
                        // 1=G1S 
                        // 2=PC without DSR 
                        // 3=PC with DSR
						// 4=PC without DTR
} G1S_PARAM;

// Protocol function
void G1S_Protocol(DWORD nCh, DWORD nCmd);

#endif
