
//=============================================================================
//                       Altius-IT Solutions
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             Farsi_dspFont_06BN08.c
//      Farsi display extended font definition
//
//=============================================================================

#include "osclib.h"         // For NULL definition
#include "Farsi_dspFont_06BN08.h" 

//=============================================================================
// Private defines and typedefs
//=============================================================================

#define CHAR_BITMAP_SIZE    8


// Macros to convert hex to binary, 
// this makes font definition more readable

#define BIT(A,B) ((((A)>>(B*4))&1)<<B)
#define B0(A) BIT(A,0)
#define B1(A) BIT(A,1)
#define B2(A) BIT(A,2)
#define B3(A) BIT(A,3)
#define B4(A) BIT(A,4)
#define B5(A) BIT(A,5)
#define B6(A) BIT(A,6)
#define B7(A) BIT(A,7)
#define B8(A) BIT(A,8)
#define B9(A) BIT(A,9)
#define B10(A) BIT(A,10)
#define B11(A) BIT(A,11)
#define B12(A) BIT(A,12)
#define B13(A) BIT(A,13)
#define B14(A) BIT(A,14)
#define B15(A) BIT(A,15)
#define L(A) (B0(A)|B1(A)|B2(A)|B3(A)|B4(A)|B5(A)|B6(A)|B7(A)|B8(A)|B9(A)|B10(A)|B11(A)|B12(A)|B13(A)|B14(A)|B15(A))

//=============================================================================
// Private data declarations
//=============================================================================

static const FONT_SECTION_HEADER section1;
static const unsigned char char80[];

//=============================================================================
// Public data definitions
//=============================================================================


//--------------------------------------------------
// Font header definition
//--------------------------------------------------
// Extended Font header
const FONT_HEADER FONT_EX_FA_06BN08_DEF =
{
    FONT_FMT_ASCII,     // format
    CHAR_BITMAP_SIZE,   // bitmapSize
    7,                  // width
    8,                  // height
    0,                  // charSpace
    1,                  // rowSpace
    &section1           // pSection
};


//=============================================================================
// Private data definitions
//=============================================================================

//--------------------------------------------------
// Section 1 (0x80 - 0xFF)
//--------------------------------------------------
static const FONT_SECTION_HEADER section1 =
{
    0x80,           // firstChar
    0xFF,           // lastChar
    NULL,           // pNext
    char80	    // character data
};


// Character 0x80
static const unsigned char char80[] =
{

// Character 0x80 farsi 0
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00110000ULL),
    L(0x00111000ULL),
    L(0x00011000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x81 farsi 1
    L(0x00110000ULL),
    L(0x00110000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),
    L(0x00001100ULL),
    L(0x00001100ULL),
    L(0x00001100ULL),
    L(0x00001100ULL),

// Character 0x82 farsi 2
    L(0x01100010ULL),
    L(0x01111110ULL),
    L(0x00110000ULL),
    L(0x00110000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),

// Character 0x83 farsi 3
    L(0x01101010ULL),
    L(0x01111110ULL),
    L(0x00110000ULL),
    L(0x00110000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),

// Character 0x84 farsi 4
    L(0x01101010ULL),
    L(0x01110010ULL),
    L(0x00111110ULL),
    L(0x00110000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),
    L(0x00011000ULL),

// Character 0x85 farsi 5
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00110110ULL),
    L(0x01100110ULL),
    L(0x01111110ULL),
    L(0x00111100ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x86 farsi 6
    L(0x00111110ULL),
    L(0x00100000ULL),
    L(0x00111110ULL),
    L(0x00001100ULL),
    L(0x00011000ULL),
    L(0x00110000ULL),
    L(0x00110000ULL),
    L(0x00110000ULL),

// Character 0x87 farsi 7
    L(0x01100010ULL),
    L(0x01100010ULL),
    L(0x00110010ULL),
    L(0x00110010ULL),
    L(0x00011110ULL),
    L(0x00011100ULL),
    L(0x00001100ULL),
    L(0x00001100ULL),

// Character 0x88 farsi 8
    L(0x00110000ULL),
    L(0x00110000ULL),
    L(0x00111000ULL),
    L(0x01111000ULL),
    L(0x01001100ULL),
    L(0x01001100ULL),
    L(0x01000110ULL),
    L(0x01000110ULL),

// Character 0x89 farsi 9
    L(0x00111110ULL),
    L(0x01100110ULL),
    L(0x01100110ULL),
    L(0x00111110ULL),
    L(0x00000110ULL),
    L(0x00000110ULL),
    L(0x00000110ULL),
    L(0x00000110ULL),

// Character 0x8A
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x8B
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x8C
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x8D
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x8E
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x8F
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x90
    L(0x11111111ULL),
    L(0x11000011ULL),
    L(0x10100101ULL),
    L(0x10011001ULL),
    L(0x10011001ULL),
    L(0x10100101ULL),
    L(0x11000011ULL),
    L(0x11111111ULL),

// Character 0x91 alef 1 allone and initial
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x92 alef 2 allone and initial
    L(0x00111000ULL),
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x93 alef middle and end
    L(0x00000000ULL),
    L(0x01000000ULL),
    L(0x01000000ULL),
    L(0x01000000ULL),
    L(0x01000000ULL),
    L(0x00111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x94 be allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01100001ULL),
    L(0x11000001ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00001000ULL),

// Character 0x95 be initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00100000ULL),

// Character 0x96 be middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00010000ULL),

// Character 0x97 be end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01000100ULL),
    L(0x10000100ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00010000ULL),

// Character 0x98 pe allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01000001ULL),
    L(0x10000001ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00101010ULL),

// Character 0x99 pe initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x01010100ULL),

// Character 0x9A pe middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x10101000ULL),

// Character 0x9B pe end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01000010ULL),
    L(0x10000010ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x01010100ULL),

// Character 0x9C te allone
    L(0x00000000ULL),
    L(0x00100100ULL),
    L(0x00000000ULL),
    L(0x01000001ULL),
    L(0x10000001ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x9D te initial
    L(0x00000000ULL),
    L(0x00010100ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x9E te middle
    L(0x00000000ULL),
    L(0x00010100ULL),
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0x9F te end
    L(0x00000000ULL),
    L(0x00001010ULL),
    L(0x00000000ULL),
    L(0x01000010ULL),
    L(0x10000010ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xA0 se allone
    L(0x00001000ULL),
    L(0x00100010ULL),
    L(0x00000000ULL),
    L(0x01000001ULL),
    L(0x10000001ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xA1 se initial
    L(0x00001000ULL),
    L(0x00100010ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xA2 se middle
    L(0x00001000ULL),
    L(0x00100010ULL),
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xA3 se end
    L(0x00001000ULL),
    L(0x00100010ULL),
    L(0x00000000ULL),
    L(0x01000010ULL),
    L(0x10000010ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xA4 jim allone
    L(0x00000000ULL),
    L(0x01111111ULL),
    L(0x00011000ULL),
    L(0x01100000ULL),
    L(0x10000000ULL),
    L(0x10001000ULL),
    L(0x01000000ULL),
    L(0x00111100ULL),

// Character 0xA5 jim initial and middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00000010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00010000ULL),

// Character 0xA6 jim end
    L(0x00000000ULL),
    L(0x01111111ULL),
    L(0x00011010ULL),
    L(0x01100010ULL),
    L(0x10000010ULL),
    L(0x10010011ULL),
    L(0x01000000ULL),
    L(0x00111100ULL),

// Character 0xA7 che allone and end
    L(0x00000000ULL),
    L(0x01111111ULL),
    L(0x00011000ULL),
    L(0x01100010ULL),
    L(0x10001000ULL),
    L(0x10000010ULL),
    L(0x01000000ULL),
    L(0x00111100ULL),

// Character 0xA8 che initial and middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00000010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x01010100ULL),

// Character 0xA9 he allone
    L(0x00000000ULL),
    L(0x01111111ULL),
    L(0x00011000ULL),
    L(0x01100000ULL),
    L(0x10000000ULL),
    L(0x10000000ULL),
    L(0x01000000ULL),
    L(0x00111100ULL),

// Character 0xAA he initial and middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00000010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xAB he end
    L(0x00000000ULL),
    L(0x01111111ULL),
    L(0x00011010ULL),
    L(0x01100010ULL),
    L(0x10000010ULL),
    L(0x10000011ULL),
    L(0x01000000ULL),
    L(0x00111100ULL),

// Character 0xAC khe allone
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x11111110ULL),
    L(0x00110000ULL),
    L(0x01000000ULL),
    L(0x10000000ULL),
    L(0x01000000ULL),
    L(0x00111100ULL),

// Character 0xAD khe initial and middle
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00000010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xAE khe end
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x11111110ULL),
    L(0x00110100ULL),
    L(0x01000100ULL),
    L(0x10000011ULL),
    L(0x01000000ULL),
    L(0x00111100ULL),

// Character 0xAF dal all cases
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xB0 zal all cases
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xB1 re all cases
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x10001000ULL),
    L(0x01110000ULL),

// Character 0xB2 ze all cases
    L(0x00000000ULL),
    L(0x00100000ULL),
    L(0x00001000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x10001000ULL),
    L(0x01110000ULL),

// Character 0xB3 zhe all cases
    L(0x00010000ULL),
    L(0x01000100ULL),
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x10001000ULL),
    L(0x01110000ULL),

// Character 0xB4 left part of sin/shin/sad/zad
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01000001ULL),
    L(0x10000001ULL),
    L(0x10000001ULL),
    L(0x01111110ULL),
    L(0x00000000ULL),

// Character 0xB5 sin allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00010010ULL),
    L(0x00010010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xB6 sin initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01001001ULL),
    L(0x01001001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),


// Character 0xB7 sin middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01001001ULL),
    L(0x01001001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xB8 sin end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00100100ULL),
    L(0x00100100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xB9 shin allone
    L(0x00001000ULL),
    L(0x00100010ULL),
    L(0x00000000ULL),
    L(0x00010010ULL),
    L(0x00010010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xBA shin initial
    L(0x00001000ULL),
    L(0x00100010ULL),
    L(0x00000000ULL),
    L(0x01001001ULL),
    L(0x01001001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xBB shin middle
    L(0x00010000ULL),
    L(0x01000100ULL),
    L(0x00000000ULL),
    L(0x01001010ULL),
    L(0x01001010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xBC shin end
    L(0x00010000ULL),
    L(0x01000100ULL),
    L(0x00000000ULL),
    L(0x00100100ULL),
    L(0x00100100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xBD sad allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00011111ULL),
    L(0x00100001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xBE sad initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01001111ULL),
    L(0x01010001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xBF sad middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01001110ULL),
    L(0x01010010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC0 sad end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00111100ULL),
    L(0x01000100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC1 zad allone
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x00111110ULL),
    L(0x01000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC2 zad initial
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x01001111ULL),
    L(0x01010001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC3 zad middle
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x01001110ULL),
    L(0x01010010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC4 zad end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00111100ULL),
    L(0x01000100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC5 ta allone and initial
    L(0x00000000ULL),
    L(0x00100000ULL),
    L(0x00100000ULL),
    L(0x00100111ULL),
    L(0x00101001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC6 ta middle and end
    L(0x00000000ULL),
    L(0x01000000ULL),
    L(0x01000000ULL),
    L(0x01001110ULL),
    L(0x01010010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC7 za allone and initial
    L(0x00000000ULL),
    L(0x00100010ULL),
    L(0x00100000ULL),
    L(0x00100111ULL),
    L(0x00101001ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC8 za middle and end
    L(0x00000000ULL),
    L(0x01000100ULL),
    L(0x01000000ULL),
    L(0x01001110ULL),
    L(0x01010010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xC9 eyn allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00100000ULL),
    L(0x01111000ULL),
    L(0x10000000ULL),
    L(0x01000000ULL),
    L(0x00111000ULL),

// Character 0xCA eyn initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00111100ULL),
    L(0x00100000ULL),
    L(0x11111100ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xCB eyn middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00010100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xCC eyn end
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00101000ULL),
    L(0x00111000ULL),
    L(0x01000100ULL),
    L(0x10000011ULL),
    L(0x01000000ULL),
    L(0x00111000ULL),

// Character 0xCD qeyn allone
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00100000ULL),
    L(0x01111000ULL),
    L(0x10000000ULL),
    L(0x01000000ULL),
    L(0x00111000ULL),

// Character 0xCE qeyn initial
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00100000ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xCF qeyn middle
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00101000ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xD0 qeyn end
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00101000ULL),
    L(0x01111000ULL),
    L(0x10000111ULL),
    L(0x01000000ULL),
    L(0x00111000ULL),

// Character 0xD1 fe allone
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x01000111ULL),
    L(0x10000101ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xD2 fe initial
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00101000ULL),
    L(0x11111000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xD3 fe middle
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00010100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xD4 fe end
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000000ULL),
    L(0x10001110ULL),
    L(0x10001010ULL),
    L(0x01111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xD5 qaf allone
    L(0x00000000ULL),
    L(0x00010100ULL),
    L(0x00000000ULL),
    L(0x00000111ULL),
    L(0x10000101ULL),
    L(0x10000111ULL),
    L(0x10000001ULL),
    L(0x01111110ULL),

// Character 0xD6 qaf initial
    L(0x00010100ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00010100ULL),
    L(0x00011100ULL),
    L(0x11111100ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xD7 qaf middle
    L(0x00010100ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00010100ULL),
    L(0x00011100ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xD8 qaf end
    L(0x00000000ULL),
    L(0x00001010ULL),
    L(0x00000000ULL),
    L(0x10001110ULL),
    L(0x10001010ULL),
    L(0x10001111ULL),
    L(0x10000010ULL),
    L(0x01111100ULL),

// Character 0xD9 kaf allone
    L(0x00000000ULL),
    L(0x00000001ULL),
    L(0x00000010ULL),
    L(0x10000100ULL),
    L(0x10000010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xDA kaf initial
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00001000ULL),
    L(0x00010000ULL),
    L(0x00001000ULL),
    L(0x11111100ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xDB kaf middle
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000100ULL),
    L(0x00001000ULL),
    L(0x00001100ULL),
    L(0x11110011ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xDC kaf end
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000100ULL),
    L(0x10001000ULL),
    L(0x10001100ULL),
    L(0x11110011ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),


// Character 0xDD gaf allone
    L(0x00000010ULL),
    L(0x00000101ULL),
    L(0x00001010ULL),
    L(0x10000100ULL),
    L(0x10000010ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xDE gaf initial
    L(0x00001000ULL),
    L(0x00010100ULL),
    L(0x00101000ULL),
    L(0x00010000ULL),
    L(0x00001000ULL),
    L(0x11111100ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xDF gaf middle
    L(0x00000100ULL),
    L(0x00001010ULL),
    L(0x00010100ULL),
    L(0x00001000ULL),
    L(0x00001100ULL),
    L(0x11110011ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xE0 gaf end
    L(0x00000100ULL),
    L(0x00001010ULL),
    L(0x00010100ULL),
    L(0x10001000ULL),
    L(0x10001100ULL),
    L(0x11110011ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xE1 lam allone
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x10000010ULL),
    L(0x10000010ULL),
    L(0x01111100ULL),
    L(0x00000000ULL),

// Character 0xE2 lam initial
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xE3 lam middle
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xE4 lam end
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x00000100ULL),
    L(0x10000100ULL),
    L(0x10000111ULL),
    L(0x01111000ULL),
    L(0x00000000ULL),

// Character 0xE5 mim allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x01110000ULL),
    L(0x00001000ULL),
    L(0x11111000ULL),
    L(0x01000000ULL),
    L(0x01000000ULL),

// Character 0xE6 mim initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000110ULL),
    L(0x00001001ULL),
    L(0x11110111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xE7 mim middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00001100ULL),
    L(0x00010010ULL),
    L(0x11101111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xE8 mim end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00000100ULL),
    L(0x11111111ULL),
    L(0x01000000ULL),
    L(0x01000000ULL),

// Character 0xE9 nun allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x10000010ULL),
    L(0x10000010ULL),
    L(0x10000010ULL),
    L(0x01111100ULL),

// Character 0xEA nun initial
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xEB nun middle
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00010000ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xEC nun end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00010000ULL),
    L(0x00000000ULL),
    L(0x10000100ULL),
    L(0x10000111ULL),
    L(0x10000100ULL),
    L(0x01111000ULL),

// Character 0xED vav all cases
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00011100ULL),
    L(0x00010100ULL),
    L(0x00011100ULL),
    L(0x00000100ULL),
    L(0x00001000ULL),
    L(0x11110000ULL),

// Character 0xEE he2 allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00100100ULL),
    L(0x00011000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xEF he2 initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00110000ULL),
    L(0x00101000ULL),
    L(0x11110111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xF0 he2 middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00111000ULL),
    L(0x00101000ULL),
    L(0x11111111ULL),
    L(0x00101000ULL),
    L(0x00111100ULL),

// Character 0xF1 he2 end
    L(0x00000000ULL),
    L(0x00000100ULL),
    L(0x00011100ULL),
    L(0x00010100ULL),
    L(0x00011100ULL),
    L(0x00000111ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xF2 ye allone
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00011111ULL),
    L(0x00010000ULL),
    L(0x10011100ULL),
    L(0x10000100ULL),
    L(0x01111100ULL),

// Character 0xF3 ye initial
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000010ULL),
    L(0x00000010ULL),
    L(0x11111110ULL),
    L(0x00000000ULL),
    L(0x00101000ULL),

// Character 0xF4 ye middle
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00001000ULL),
    L(0x00001000ULL),
    L(0x11111111ULL),
    L(0x00000000ULL),
    L(0x01010000ULL),

// Character 0xF5 ye end
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x10000000ULL),
    L(0x10001111ULL),
    L(0x01111100ULL),
    L(0x00101000ULL),

// Character 0xF6
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xF7
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xF8
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xF9
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xFA
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xFB
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xFC
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xFD
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xFE
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),

// Character 0xFF
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
    L(0x00000000ULL),
};

