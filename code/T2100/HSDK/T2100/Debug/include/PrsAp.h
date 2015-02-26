
///////////////////////////////////////////////////////////////////////////
//
// Module   : PRSAP
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module PRSAP
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
// 020712  BW      Original version (based on EFTOS7 version 980729)
// 030611  BW      Major change of handling of serial ports
//
///////////////////////////////////////////////////////////////////////////

#ifndef PRSAP_H
#define PRSAP_H

// Protocol Schedular application interface functions
void S_Cancel(DWORD nCh);
void S_CancelInBg(DWORD nCh);
void S_Read(LPBYTE pBuf, DWORD nBufLen, DWORD nCh);
void S_Setup(DWORD nCh, DWORD nBaud, LPBYTE pParam, LPVOID pProtEntry);
void S_Terminate(DWORD nCh);
void S_Write(LPBYTE pBuf, DWORD nBufLength, DWORD nCh);

#endif
