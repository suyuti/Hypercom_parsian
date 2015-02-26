
///////////////////////////////////////////////////////////////////////////
//
// Module   : FSH
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file for FSH
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
// 030428  BW      Original version (based on EFTOS8 version 030414)
// 030513  BW      Functions to retrieve start and end sectors are made
//                 public.
// 050302  RZ      Added functionality for AM
//
///////////////////////////////////////////////////////////////////////////

#ifndef FSH_H
#define FSH_H

// Return codes from operations
#define FSH_OK                  0
#define FSH_ERR_NOT_RAM         1   // obsolete
#define FSH_ERR_ERASE           2
#define FSH_ERR_PGM             3
#define FSH_ERR_SECTOR_RANGE    4
#define FSH_ERR_LOCK			5
#define FSH_PROCESS				6
#define FSH_ERR_NO_FREE			-1
#define FSH_ERR_GENERAL			-1

// Flash memory page status
#define FSH_PAGE_FREE	'F'
#define FSH_PAGE_USED	'U'
#define FSH_PAGE_LOCK	'L'

// Available operations
void  FSH_Init(void);
void  FSH_BuildMemoryMap(void);
const char* FSH_RetMemoryMap(void);
DWORD FSH_RetFlashType(void);
DWORD FSH_GetFreeSector( void );
DWORD FSH_GetSectorAddr( DWORD nSector );
DWORD FSH_GetSectorNumber( DWORD nAddr );
DWORD FSH_EraseSector(DWORD nSector);
DWORD FSH_EraseAppl(void);
DWORD FSH_EraseData(void);
DWORD FSH_RetAddrAppEnd(void);
DWORD FSH_RetAddrAppStart(void);
DWORD FSH_RetAddrDataEnd(void);
DWORD FSH_RetAddrDataStart(void);
DWORD FSH_RetSectorAppEnd(void);
DWORD FSH_RetSectorAppStart(void);
DWORD FSH_RetSectorDataEnd(void);
DWORD FSH_RetSectorDataStart(void);
DWORD FSH_Pgm(DWORD Dest, LPBYTE pSource, DWORD DataSize);

#endif
