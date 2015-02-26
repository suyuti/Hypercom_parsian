#ifdef MAKE_FARSI

#ifndef __inc_Farsi_print
#define __inc_Farsi_print
//=============================================================================
//                       Altius-IT
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:            Farsi_print.h
//      Print support for Farsi
//=============================================================================

// Some definitions provided in prtdrv
#define chrNORMAL  0x02
#define chrDOUBLE  0x17

// Printer definitions
#define Farsi_max_char_per_line 32
#define maxDotPerLine 384

// Font definitions
//#define CharHeight 28 // T2100/M2100
#define CharHeight 14 // T42xx MT101 printer
#define InterLineSpace 2 // space between character lines, in dots

// Type to define single printable character
typedef struct
{
    const UBYTE CharCode;
    const UBYTE CharWdth;
    const UBYTE CharOptions;
    const UWORD CharBitMap[CharHeight];
}
__attribute__((packed)) CharDefinition;


// Parser definitions
#define FixedBufLength 80




// Routines for user interface
extern OS_RETURN_VALS Farsi_Print( UBYTE *pInpBuff, UWORD InpLen );
extern void Farsi_PrintCharacterSet( void );



#endif // not __inc_Farsi_print

#endif // MAKE_FARSI
