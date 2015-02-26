
///////////////////////////////////////////////////////////////////////////
//
// Module   : PRN
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module PRN
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
// 030501  BW      Original version
// 030912  BW      Added functions for change of printer font.
//
///////////////////////////////////////////////////////////////////////////

#ifndef PRINTER_H
#define PRINTER_H

// Define character code for first and last character in printer font table
// that can be defined and printed. The defafult table is only defined
// for characters 0x20-0x9f and characters above 0x9f will give blanks.
// New font set can be defined for characters above 0x9f.
// Char 0xff can not be printed and shall not be defined.
#define PRN_FIRST_CHAR                  0x20
#define PRN_LAST_CHAR                   0xfe

// Define number of bytes used to build up a character in the printer font table
#define PRN_NO_BYTE_PER_CHAR            8

// Function declarations
void  PRN_Cancel(void);
BOOL  PRN_DefAllCharDefault(DWORD nTimer);
BOOL  PRN_DefChar(BYTE nFirstChar, DWORD nNoChar, LPBYTE pData, DWORD nTimer);
BOOL  PRN_DefCharDefault(BYTE nFirstChar, DWORD nNoChar, DWORD nTimer);
BYTE  PRN_GetPrinterStatus(void);
void  PRN_Init(void);
DWORD PRN_PrinterCmd(const LPBYTE pCmdArr, DWORD nCmdLength, BYTE* pResponse);
DWORD PRN_PrintStart(const LPBYTE pData, DWORD nLength);
BOOL* PRN_RetPrintRdyEvtPtr(void);

void  PRN_HwReset(void);


#endif
