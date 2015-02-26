#ifndef _PRTDETAYLI_H_
#define _PRTDETAYLI_H_
//=============================================================================
//                       Servus Bilgisayar
//                      (c) Copyright 2007
//                       Erdinc Taskin 05.03.2007
//                       ptrDetayli.h
//=============================================================================
//
// Veritaban� tablolar�n� ayr�nt�l� bi�imde yazd�rmak i�in




//#include "img.h"
#define PRINTTYPE_NUMBER 'N'
#define PRINTTYPE_ASCII 'A'
#define PRINTTYPE_RAW 'R'
#define PRINTTYPE_BOOL 'B'
#define PRINTTYPE_BYTE 'T'
#define PRINTTYPE_NOTPRINT 'X'

extern void printTerminalTable();
extern void printAcquirerTable();
extern void printIssuerTable();
extern void printCardRangeTable();
extern void PrtFormattedDB( );

#endif //_PTRDETAYLI_H_
