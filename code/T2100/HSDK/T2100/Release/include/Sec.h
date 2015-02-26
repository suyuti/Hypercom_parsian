
///////////////////////////////////////////////////////////////////////////
//
// Module   : SEC
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module SEC
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
// 030613  BW      Original version (based on EFTOS8 version 020712)
//
///////////////////////////////////////////////////////////////////////////

#ifndef SEC_H
#define SEC_H

// Structure holding data transferred to and from application
typedef struct {
    DWORD   nCmd;       // Command  
    LPBYTE  pAD;        // Address (pointer) to application data buffer
    DWORD   nCnt;       // Buffer size
    BOOL    bEvt;       // Event flag to indicate when command is ready
    DWORD   nRetStat;   // Return status
} SEC_MOD_DATA;

extern SEC_MOD_DATA volatile SEC_RdData;
extern SEC_MOD_DATA volatile SEC_WrData;


// Security Module functions
void SEC_Cancel(void);
void SEC_NoReset(void);
void SEC_Open(DWORD nBaudRate);
void SEC_Read(LPBYTE pBuffer, DWORD nNoBytes);
void SEC_Reset(void);
void SEC_Scan(void);
void SEC_Write(LPBYTE pBuffer, DWORD nNoBytes);
BOOL SEC_Version(LPBYTE pBuffer,DWORD nNoBytes);
void SEC_Close(void);

#endif
