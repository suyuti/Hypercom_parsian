#ifdef MAKE_FARSI

//=============================================================================
//                           Altius-It
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             Farsi_prnFontNormal.c
//      Support print for Farsi
//=============================================================================

#include <string.h>
#include <stdlib.h>
#include "basictyp.h"
#include "sdk.h" // OS_RETURN_VALS

#include "Farsi_print.h"
#include "Farsi_prnCodePage.h"
#include "Farsi_prnFontNormal.h"


// Macros to convert long hex to binary, this makes font definition more readable
// NOTE: number of entries must be equal CharHeight

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


// Symbol draving definition tables

const CharDefinition c_Space=
{
    0x20,  
    10,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Exclam=
{
    0x21,
    8,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0000000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
    },
};

const CharDefinition c_Quotes=
{
    0x22,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x011100111000ULL),
        L(0x001100011000ULL),
        L(0x001000010000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Grate=
{
    0x23,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001100011000ULL),
        L(0x001100011000ULL),
        L(0x111111111110ULL),
        L(0x001100011000ULL),
        L(0x001100011000ULL),
        L(0x001100011000ULL),
        L(0x001100011000ULL),
        L(0x111111111110ULL),
        L(0x001100011000ULL),
        L(0x001100011000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Dollar=
{
    0x24,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x011111111000ULL),
        L(0x111011001100ULL),
        L(0x110011000000ULL),
        L(0x111011000000ULL),
        L(0x001111111000ULL),
        L(0x000011001100ULL),
        L(0x000011001100ULL),
        L(0x110011001100ULL),
        L(0x111011011100ULL),
        L(0x001111111000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Prcnt=
{
    0x25,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x011110000110ULL),
        L(0x110011001100ULL),
        L(0x011110011000ULL),
        L(0x000000110000ULL),
        L(0x000001100000ULL),
        L(0x000011000000ULL),
        L(0x000110000000ULL),
        L(0x001100111100ULL),
        L(0x011001100110ULL),
        L(0x110000111100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_At=
{
    0x26,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000111110000ULL),
        L(0x001100011000ULL),
        L(0x001100011000ULL),
        L(0x000110110000ULL),
        L(0x000011000000ULL),
        L(0x001101100000ULL),
        L(0x110000110010ULL),
        L(0x110000011100ULL),
        L(0x111000111100ULL),
        L(0x001111100110ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Apstr=
{
    0x27,
    8,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x01110000ULL),
        L(0x01110000ULL),
        L(0x00100000ULL),
        L(0x01000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
    },
};

const CharDefinition c_Lbrck=
{
    0x28,
    9,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000111000ULL),
        L(0x001100000ULL),
        L(0x011000000ULL),
        L(0x110000000ULL),
        L(0x110000000ULL),
        L(0x110000000ULL),
        L(0x110000000ULL),
        L(0x011000000ULL),
        L(0x001100000ULL),
        L(0x000111000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

const CharDefinition c_Rbrck=
{
    0x29,
    9,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x111000000ULL),
        L(0x001100000ULL),
        L(0x000110000ULL),
        L(0x000011000ULL),
        L(0x000011000ULL),
        L(0x000011000ULL),
        L(0x000011000ULL),
        L(0x000110000ULL),
        L(0x001100000ULL),
        L(0x111000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

const CharDefinition c_Astr=
{
    0x2A,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x110011001100ULL),
        L(0x011011011000ULL),
        L(0x001111110000ULL),
        L(0x000111100000ULL),
        L(0x111111111100ULL),
        L(0x000111100000ULL),
        L(0x001111110000ULL),
        L(0x011011011000ULL),
        L(0x110011001100ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Plus=
{
    0x2B,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x111111111100ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Comma=
{
    0x2C,
    7,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x1110000ULL),
        L(0x0110000ULL),
        L(0x0100000ULL),
        L(0x0000000ULL),
    },
};

const CharDefinition c_Minus=
{
    0x2D,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Dot=
{
    0x2E,
    8,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00111100ULL),
        L(0x00111100ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
    },
};

const CharDefinition c_Slsh=
{
    0x2F,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000110ULL),
        L(0x000000001100ULL),
        L(0x000000011000ULL),
        L(0x000000110000ULL),
        L(0x000001100000ULL),
        L(0x000011000000ULL),
        L(0x000110000000ULL),
        L(0x001100000000ULL),
        L(0x011000000000ULL),
        L(0x110000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};


const CharDefinition c_0=
{
    0x30, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000011100ULL),
        L(0x110000101100ULL),
        L(0x110001001100ULL),
        L(0x110010001100ULL),
        L(0x110100001100ULL),
        L(0x111000001100ULL),
        L(0x011000011000ULL),
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_1=
{  
    0x31,  
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x001111000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_2=
{
    0x32,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x000000001100ULL),
        L(0x000000011000ULL),
        L(0x000001100000ULL),
        L(0x000011000000ULL),
        L(0x001110000000ULL),
        L(0x011000000000ULL),
        L(0x111111111100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_3=
{
    0x33,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x000000011000ULL),
        L(0x000111100000ULL),
        L(0x000000111000ULL),
        L(0x000000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_4=
{
    0x34, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000011100ULL),
        L(0x000001111100ULL),
        L(0x000011001100ULL),
        L(0x001110001100ULL),
        L(0x011000001100ULL),
        L(0x110000001100ULL),
        L(0x111111111100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_5=
{
    0x35,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x111111110000ULL),
        L(0x110000011000ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x110000001100ULL),
        L(0x111000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_6=
{
    0x36,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111111000ULL),
        L(0x011000001100ULL),
        L(0x110000000000ULL),
        L(0x111111110000ULL),
        L(0x110000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_7=
{
    0x37,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x000000001100ULL),
        L(0x000000011000ULL),
        L(0x000000110000ULL),
        L(0x000001100000ULL),
        L(0x000011000000ULL),
        L(0x000110000000ULL),
        L(0x001100000000ULL),
        L(0x011000000000ULL),
        L(0x110000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_8=
{
    0x38,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_9=
{
    0x39,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000001100ULL),
        L(0x001111111100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Colon=
{
    0x3A,
    7,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
    },
};

const CharDefinition c_Smcln=
{
    0x3B,
    7,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0111000ULL),
        L(0x0111000ULL),
        L(0x0001000ULL),
        L(0x0010000ULL),
    },
};

const CharDefinition c_Less=
{
    0x3C,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000100ULL),
        L(0x000000011100ULL),
        L(0x000001110000ULL),
        L(0x000111000000ULL),
        L(0x011100000000ULL),
        L(0x011100000000ULL),
        L(0x000111000000ULL),
        L(0x000001110000ULL),
        L(0x000000011100ULL),
        L(0x000000000100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Equal=
{
    0x3D,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_More=
{
    0x3E,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x100000000000ULL),
        L(0x111000000000ULL),
        L(0x001110000000ULL),
        L(0x000011100000ULL),
        L(0x000000111000ULL),
        L(0x000000111000ULL),
        L(0x000011100000ULL),
        L(0x001110000000ULL),
        L(0x111000000000ULL),
        L(0x100000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Quest=
{
    0x3F,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x000000001100ULL),
        L(0x000000011000ULL),
        L(0x000001100000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Dog=
{
    0x40,
    14,  // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00111111100000ULL),
        L(0x11000000111000ULL),
        L(0x00000000001100ULL),
        L(0x01111100001100ULL),
        L(0x11000110001100ULL),
        L(0x11000110001100ULL),
        L(0x11000110001100ULL),
        L(0x11000110001100ULL),
        L(0x11101110011000ULL),
        L(0x00111011110000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_A=
{
    0x41, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x000111100000ULL),
        L(0x001100110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x111111111100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_B=
{
    0x42,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111100000ULL),
        L(0x110000111000ULL),
        L(0x110000011000ULL),
        L(0x110000110000ULL),
        L(0x111111110000ULL),
        L(0x110000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000011000ULL),
        L(0x111111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_C=
{
    0x43,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_D=
{
    0x44,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111110000ULL),
        L(0x011000011000ULL),
        L(0x011000001100ULL),
        L(0x011000001100ULL),
        L(0x011000001100ULL),
        L(0x011000001100ULL),
        L(0x011000001100ULL),
        L(0x011000001100ULL),
        L(0x011000011000ULL),//
        L(0x111111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_E=
{
    0x45,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x111111110000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x111111111100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_F=
{
    0x46,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x111111110000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_G=
{
    0x47,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110001111100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_H=
{
    0x48, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x111111111100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_I=
{
    0x49, 
    8,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x11111100ULL),
        L(0x00110000ULL),
        L(0x00110000ULL),
        L(0x00110000ULL),
        L(0x00110000ULL),
        L(0x00110000ULL),
        L(0x00110000ULL),
        L(0x00110000ULL),
        L(0x00110000ULL),
        L(0x11111100ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
    },
};

const CharDefinition c_J=
{
    0x4A,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000111100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_K=
{
    0x4B, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x110000011000ULL),
        L(0x110000110000ULL),
        L(0x110001100000ULL),
        L(0x110011000000ULL),
        L(0x110111000000ULL),
        L(0x111101100000ULL),
        L(0x110000110000ULL),
        L(0x110000011000ULL),
        L(0x110000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_L=
{
    0x4C, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x111111111100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_M=
{
    0x4D, 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x1100000000001100ULL),
        L(0x1110000000011100ULL),
        L(0x1111000000111100ULL),
        L(0x1101100001101100ULL),
        L(0x1100110011001100ULL),
        L(0x1100011110001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100000000001100ULL),
        L(0x1100000000001100ULL),
        L(0x1100000000001100ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_N=
{
    0x4E, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x111000001100ULL),
        L(0x111100001100ULL),
        L(0x111110001100ULL),
        L(0x110111001100ULL),
        L(0x110011101100ULL),
        L(0x110001111100ULL),
        L(0x110000111100ULL),
        L(0x110000011100ULL),
        L(0x110000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_O=
{
    0x4F,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_P=
{
    0x50,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111110000ULL),
        L(0x110000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000011000ULL),
        L(0x111111110000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Q=
{
    0x51,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110011001100ULL),
        L(0x011001111000ULL),
        L(0x001111001110ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_R=
{
    0x52,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111110000ULL),
        L(0x110000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000111000ULL),
        L(0x111111100000ULL),
        L(0x110001100000ULL),
        L(0x110000110000ULL),
        L(0x110000011000ULL),
        L(0x110000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_S=
{
    0x53,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000000000ULL),
        L(0x011000000000ULL),
        L(0x001111110000ULL),
        L(0x000000011000ULL),
        L(0x000000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_T=
{
    0x54, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_U=
{
    0x55,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),//
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_V=
{
    0x56,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),
        L(0x001100110000ULL),
        L(0x000111100000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_W=
{
    0x57,
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x1100000000001100ULL),
        L(0x1100000000001100ULL),
        L(0x1100000000001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100011110001100ULL),
        L(0x1100110011001100ULL),
        L(0x1111100001111100ULL),
        L(0x0111000000111000ULL),
        L(0x0011000000110000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_X=
{
    0x58,
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x1100000001100ULL),
        L(0x0110000011000ULL),
        L(0x0011000110000ULL),
        L(0x0001101100000ULL),
        L(0x0000111000000ULL),
        L(0x0000111000000ULL),
        L(0x0001101100000ULL),
        L(0x0011000110000ULL),
        L(0x0110000011000ULL),
        L(0x1100000001100ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};

const CharDefinition c_Y=
{
    0x59,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),
        L(0x001100110000ULL),
        L(0x000111100000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Z=
{
    0x5A, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111100ULL),
        L(0x000000011000ULL),
        L(0x000000110000ULL),
        L(0x000001100000ULL),
        L(0x000011000000ULL),
        L(0x000110000000ULL),
        L(0x001100000000ULL),
        L(0x011000000000ULL),
        L(0x110000000000ULL),
        L(0x111111111100ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_LRBrk=
{
    0x5B,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000111111000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000111111000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_BkSls=
{
    0x5C,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000000000ULL),
        L(0x011000000000ULL),
        L(0x001100000000ULL),
        L(0x000110000000ULL),
        L(0x000011000000ULL),
        L(0x000001100000ULL),
        L(0x000000110000ULL),
        L(0x000000011000ULL),
        L(0x000000001100ULL),
        L(0x000000000110ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_RRBrk=
{
    0x5D,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Cover=
{
    0x5E,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x001111110000ULL),
        L(0x111000011100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Undln=
{
    0x5F,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x111111111110ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_Pfstr=
{
    0x60,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001110000000ULL),
        L(0x001110000000ULL),
        L(0x000010000000ULL),
        L(0x000100000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_a=
{
    0x61, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x011111100000ULL),
        L(0x110000110000ULL),
        L(0x001111111000ULL),
        L(0x011000011000ULL),
        L(0x110000011000ULL),
        L(0x110000111100ULL),
        L(0x011111100100ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_b=
{
    0x62, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x111111100000ULL),
        L(0x111000110000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x111000111000ULL),
        L(0x101111100000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_c=
{
    0x63, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000011000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x011000011000ULL),
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_d=
{
    0x64, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x001111111000ULL),
        L(0x011000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x011000011000ULL),
        L(0x001111111000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_e=
{
    0x65, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111110000ULL),
        L(0x011000011000ULL),
        L(0x110000011000ULL),
        L(0x111111111000ULL),
        L(0x110000000000ULL),
        L(0x011000011000ULL),
        L(0x001111110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_f=
{
    0x66, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011111000ULL),
        L(0x001110001100ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x111111000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_g=
{
    0x67, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111111000ULL),
        L(0x011000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x011000111000ULL),
        L(0x001111111000ULL),
        L(0x000000011000ULL),
        L(0x011111110000ULL),
    },
};

const CharDefinition c_h=
{
    0x68, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x111111100000ULL),
        L(0x110000110000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_i=
{
    0x69, 
    7,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0000000ULL),
        L(0x0011000ULL),
        L(0x1111000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
    },
};

const CharDefinition c_j=
{
    0x6A, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000000000ULL),
        L(0x000000011000ULL),
        L(0x000001111000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x110000110000ULL),
        L(0x011111100000ULL),
    },
};

const CharDefinition c_k=
{
    0x6B, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000000000ULL),
        L(0x110000111000ULL),
        L(0x110001110000ULL),
        L(0x110011100000ULL),
        L(0x110111000000ULL),
        L(0x111111100000ULL),
        L(0x111000110000ULL),
        L(0x110000011000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_l=
{
    0x6C, 
    7,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0011000ULL),
        L(0x0111000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0011000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
    },
};

const CharDefinition c_m=
{
    0x6D, 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x1011111001111000ULL),
        L(0x1110001111001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100001100001100ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_n=
{
    0x6E, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x101111100000ULL),
        L(0x111000110000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_o=
{
    0x6F, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111100000ULL),
        L(0x011000110000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x011000110000ULL),
        L(0x001111100000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_p=
{
    0x70, 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x11111110000ULL),
        L(0x11000011000ULL),
        L(0x11000001100ULL),
        L(0x11000001100ULL),
        L(0x11000001100ULL),
        L(0x11000011000ULL),
        L(0x11111110000ULL),
        L(0x11000000000ULL),
        L(0x11000000000ULL),
    },
};

const CharDefinition c_q=
{
    0x71, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111111000ULL),
        L(0x011000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x011000111000ULL),
        L(0x001111111000ULL),
        L(0x000000011100ULL),
        L(0x000000011000ULL),
    },
};

const CharDefinition c_r=
{
    0x72, 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x10111111000ULL),
        L(0x11100001100ULL),
        L(0x11000000000ULL),
        L(0x11000000000ULL),
        L(0x11000000000ULL),
        L(0x11000000000ULL),
        L(0x11000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_s=
{
    0x73, 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x01111110000ULL),
        L(0x11000011000ULL),
        L(0x11000000000ULL),
        L(0x01111111000ULL),
        L(0x00000001100ULL),
        L(0x11000001100ULL),
        L(0x01111111000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_t=
{
    0x74, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x111111100000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x000110001100ULL),
        L(0x000011111000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_u=
{
    0x75, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x110000011000ULL),
        L(0x011000111000ULL),
        L(0x001111101100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_v=
{
    0x76, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),
        L(0x001100110000ULL),
        L(0x000111100000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_w=
{
    0x77, 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x1100000000001100ULL),
        L(0x1100001100001100ULL),
        L(0x1100011110001100ULL),
        L(0x1100111111001100ULL),
        L(0x1101110011101100ULL),
        L(0x0111100001111000ULL),
        L(0x0011000000110000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_x=
{
    0x78, 
    10,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x1100001100ULL),
        L(0x0110011000ULL),
        L(0x0011110000ULL),
        L(0x0001100000ULL),
        L(0x0011110000ULL),
        L(0x0110011000ULL),
        L(0x1100001100ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
    },
};

const CharDefinition c_y=
{
    0x79, 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x11000001100ULL),
        L(0x11000001100ULL),
        L(0x11000001100ULL),
        L(0x11000001100ULL),
        L(0x11000001100ULL),
        L(0x01100011100ULL),
        L(0x00111111100ULL),
        L(0x00000001100ULL),
        L(0x01111111000ULL),
    },
};

const CharDefinition c_z=
{
    0x7A,
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x11111111100ULL),
        L(0x00000110000ULL),
        L(0x00001100000ULL),
        L(0x00011000000ULL),
        L(0x00110000000ULL),
        L(0x01100000000ULL),
        L(0x11111111100ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_LFBrk=
{
    0x7B,
    9,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000011100ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x011100000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000011100ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

const CharDefinition c_Stick=
{
    0x7C, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_RFBrk=
{
    0x7D,
    9,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x011100000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000011100ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x000110000ULL),
        L(0x011100000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

const CharDefinition c_Wave=
{
    0x7E,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x011110000000ULL),
        L(0x110011001100ULL),
        L(0x000001111000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};


const CharDefinition c_fa0=
{
    0x80, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000001100000ULL),
        L(0x000011111000ULL),
        L(0x000111111000ULL),
        L(0x000001110000ULL),//
        L(0x000000100000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa1=
{  
    0x81,  
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000111000000ULL),
        L(0x000111000000ULL),
        L(0x000011000000ULL),
        L(0x000001100000ULL),
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000001000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa2=
{
    0x82,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x111000011000ULL),
        L(0x111111110000ULL),
        L(0x011000000000ULL),
        L(0x011000000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),//
        L(0x000110000000ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa3=
{
    0x83,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110011001100ULL),
        L(0x110011001100ULL),
        L(0x111111111000ULL),
        L(0x011000000000ULL),
        L(0x011000000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),//
        L(0x000110000000ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa4=
{
    0x84, 
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110001111000ULL),
        L(0x110011000000ULL),
        L(0x110110001100ULL),
        L(0x111111111000ULL),
        L(0x011000000000ULL),
        L(0x001100000000ULL),
        L(0x001100000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),//
        L(0x000110000000ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa5=
{
    0x85,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000100000000ULL),
        L(0x000110000000ULL),
        L(0x000111000000ULL),
        L(0x000001100000ULL),
        L(0x000110111000ULL),
        L(0x011000011100ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x110001111000ULL),//
        L(0x011111000000ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa6=
{
    0x86,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011100000ULL),
        L(0x001111110000ULL),
        L(0x011000000000ULL),
        L(0x110000000000ULL),
        L(0x111111111100ULL),
        L(0x000000011000ULL),
        L(0x000000110000ULL),
        L(0x000001100000ULL),
        L(0x000011000000ULL),//
        L(0x000110000000ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa7=
{
    0x87,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x011000011000ULL),
        L(0x011000011000ULL),
        L(0x001100110000ULL),
        L(0x001100110000ULL),
        L(0x000111100000ULL),
        L(0x000111100000ULL),
        L(0x000011000000ULL),//
        L(0x000011000000ULL),//
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa8=
{
    0x88,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000111100000ULL),
        L(0x000111100000ULL),
        L(0x001100110000ULL),
        L(0x001100110000ULL),
        L(0x011000011000ULL),
        L(0x011000011000ULL),
        L(0x110000001100ULL),
        L(0x110000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_fa9=
{
    0x89,
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111100000ULL),
        L(0x011000110000ULL),
        L(0x110000110000ULL),
        L(0x110000110000ULL),
        L(0x011111111000ULL),
        L(0x000000011000ULL),
        L(0x000000011000ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000001100ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};


//-------------------------------------------------
const CharDefinition c_aAlef=
{
    0x91, // alef (allone and initial) 
    7,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000100ULL),
        L(0x0011100ULL),
        L(0x0011100ULL),
        L(0x0011100ULL),
        L(0x0011100ULL),
        L(0x0011100ULL),
        L(0x0001100ULL),
        L(0x0001100ULL),
        L(0x0001100ULL), //
        L(0x0001000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
        L(0x0000000ULL),
    },
};

const CharDefinition c_iAlef=
{
    0x92, // alef (initial var.2) 
    9,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x001111100ULL),
        L(0x000000000ULL),
        L(0x000001000ULL),
        L(0x000111000ULL),
        L(0x000111000ULL),
        L(0x000111000ULL),
        L(0x000111000ULL),
        L(0x000011000ULL),
        L(0x000011000ULL), //
        L(0x000011000ULL), //
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

const CharDefinition c_mAlef=
{
    0x93, // alef (meddle and end) 
    10,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000ULL),
        L(0x0011000000ULL),
        L(0x0111000000ULL),
        L(0x0111100000ULL),
        L(0x0011100000ULL),
        L(0x0011100000ULL),
        L(0x0001100000ULL),
        L(0x0001100000ULL),
        L(0x0001111111ULL), //
        L(0x0000011111ULL), //
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aBe=
{
    0x94, // be (allone) 
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00100000000000ULL),
        L(0x11000000000110ULL),
        L(0x11000000000111ULL),
        L(0x11100000000011ULL),
        L(0x01111111111111ULL), //
        L(0x00001111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000001110000ULL),
        L(0x00000001110000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_iBe=
{
    0x95, // be (initial)
    8,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000110ULL),
        L(0x00000111ULL),
        L(0x00000011ULL),
        L(0x11111111ULL), //
        L(0x11111111ULL), //
        L(0x00000000ULL),
        L(0x00011100ULL),
        L(0x00011100ULL),
        L(0x00000000ULL),
    },
};

const CharDefinition c_mBe=
{
    0x96, // be (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000110000ULL),
        L(0x00000000111000ULL),
        L(0x00000000011000ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00011100000000ULL),
        L(0x00011100000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eBe=
{
    0x97, // be (end) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0010000000000000ULL),
        L(0x1100000000011000ULL),
        L(0x1100000000011100ULL),
        L(0x1110000000001100ULL),
        L(0x0111111111111111ULL), //
        L(0x0000111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000011100000000ULL),
        L(0x0000011100000000ULL),
        L(0x0000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aPe=
{
    0x98, // pe (allone) 
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00100000000000ULL),
        L(0x11000000000110ULL),
        L(0x11000000000111ULL),
        L(0x11100000000011ULL),
        L(0x01111111111111ULL), //
        L(0x00001111111111ULL), //
        L(0x00000000000000ULL),
        L(0x01110000000111ULL),
        L(0x01110011100111ULL),
        L(0x00000011100000ULL),
    },
};

const CharDefinition c_iPe=
{
    0x99, // pe (initial)
    10,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000110ULL),
        L(0x0000000111ULL),
        L(0x0000000011ULL),
        L(0x1111111111ULL), //
        L(0x1111111111ULL), //
        L(0x0000000000ULL),
        L(0x1100000011ULL),
        L(0x1100110011ULL),
        L(0x0000110000ULL),
    },
};

const CharDefinition c_mPe=
{
    0x9A, // pe (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000110000ULL),
        L(0x00000000111000ULL),
        L(0x00000000011000ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x11100000001110ULL),
        L(0x11100111001110ULL),
        L(0x00000111000000ULL),
    },
};

const CharDefinition c_ePe=
{
    0x9B, // pe (end) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0010000000000000ULL),
        L(0x1100000000011000ULL),
        L(0x1100000000011100ULL),
        L(0x1110000000001100ULL),
        L(0x0111111111111111ULL), //
        L(0x0000111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0011100000001110ULL),
        L(0x0011100111001110ULL),
        L(0x0000000111000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aTe=
{
    0x9C, // te (allone) 
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00111000111000ULL),
        L(0x00111000111000ULL),
        L(0x00000000000000ULL),
        L(0x11000000000110ULL),
        L(0x11000000000111ULL),
        L(0x11100000000011ULL),
        L(0x01111111111111ULL), //
        L(0x00001111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_iTe=
{
    0x9D, // te (initial)
    8,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x11101110ULL),
        L(0x11101110ULL),
        L(0x00000000ULL),
        L(0x00000111ULL),
        L(0x00000011ULL),
        L(0x00000011ULL),
        L(0x11111111ULL), //
        L(0x11111111ULL), //
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
        L(0x00000000ULL),
    },
};

const CharDefinition c_mTe=
{
    0x9E, // te (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x01110011100000ULL),
        L(0x01110011100000ULL),
        L(0x00000000000000ULL),
        L(0x00000000110000ULL),
        L(0x00000000111000ULL),
        L(0x00000000011000ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eTe=
{
    0x9F, // te (end) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0001110011100000ULL),
        L(0x0001110011100000ULL),
        L(0x0000000000000000ULL),
        L(0x1100000000011000ULL),
        L(0x1100000000011100ULL),
        L(0x1110000000001100ULL),
        L(0x0111111111111111ULL), //
        L(0x0000111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aSe=
{
    0xA0, // se (allone) 
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000110000000ULL),
        L(0x00000110000000ULL),
        L(0x00110000011000ULL),
        L(0x00110000011000ULL),
        L(0x00000000000000ULL),
        L(0x11000000000110ULL),
        L(0x11000000000111ULL),
        L(0x11100000000011ULL),
        L(0x01111111111111ULL), //
        L(0x00001111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_iSe=
{
    0xA1, // se (initial)
    10,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0001100000ULL),
        L(0x0001100000ULL),
        L(0x1100001100ULL),
        L(0x1100001100ULL),
        L(0x0000000000ULL),
        L(0x0000000110ULL),
        L(0x0000000111ULL),
        L(0x0000000011ULL),
        L(0x1111111111ULL), //
        L(0x1111111111ULL), //
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
    },
};

const CharDefinition c_mSe=
{
    0xA2, // se (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00110000000000ULL),
        L(0x00110000000000ULL),
        L(0x00000000000000ULL),
        L(0x11001100000000ULL),
        L(0x11001100000000ULL),
        L(0x00000000110000ULL),
        L(0x00000000111000ULL),
        L(0x00000000011000ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eSe=
{
    0xA3, // se (end) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000001100000000ULL),
        L(0x0000001100000000ULL),
        L(0x0011000001100000ULL),
        L(0x0011000001100000ULL),
        L(0x0000000000000000ULL),
        L(0x1100000000011000ULL),
        L(0x1100000000011100ULL),
        L(0x1110000000001100ULL),
        L(0x0111111111111111ULL), //
        L(0x0000111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aJim=
{
    0xA4, // jim (allone) 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00111111111ULL),
        L(0x00000011100ULL),
        L(0x00011110000ULL),
        L(0x01110000000ULL),
        L(0x11000011000ULL), //
        L(0x11000011000ULL), //
        L(0x01100000000ULL),
        L(0x01100000000ULL),
        L(0x00111100000ULL),
        L(0x00000111110ULL),
    },
};

const CharDefinition c_iJim=
{
    0xA5, // jim (initial and middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00011111110000ULL),
        L(0x00000001111000ULL),
        L(0x00000000011100ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000111000000ULL),
        L(0x00000111000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eJim=
{
    0xA6, // jim (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0111111111100ULL),
        L(0x0000001110000ULL),
        L(0x0001110110000ULL),
        L(0x0111000110000ULL),
        L(0x1100000011111ULL), //
        L(0x1100011000111ULL), //
        L(0x0110011000000ULL),
        L(0x0011000000000ULL),
        L(0x0001111000000ULL),
        L(0x0000011111000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aChe=
{
    0xA7, // che (allone and end) 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00111111111ULL),
        L(0x00000011110ULL),
        L(0x00011110000ULL),
        L(0x01110000011ULL),
        L(0x11000110011ULL), //
        L(0x11000110000ULL), //
        L(0x01100000011ULL),
        L(0x00110000011ULL),
        L(0x00011100000ULL),
        L(0x00000111110ULL),
    },
};

const CharDefinition c_iChe=
{
    0xA8, // che (initial and middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00011111110000ULL),
        L(0x00000001111000ULL),
        L(0x00000000011100ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x01100000011000ULL),
        L(0x01100110011000ULL),
        L(0x00000110000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aHe=
{
    0xA9, // he (allone) 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00111111111ULL),
        L(0x00000011100ULL),
        L(0x00011110000ULL),
        L(0x01110000000ULL),
        L(0x11000000000ULL), //
        L(0x11000000000ULL), //
        L(0x01100000000ULL),
        L(0x01100000000ULL),
        L(0x00111000000ULL),
        L(0x00000111110ULL),
    },
};

const CharDefinition c_iHe=
{
    0xAA, // he (initial and middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00011111110000ULL),
        L(0x00000001111000ULL),
        L(0x00000000011100ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eHe=
{
    0xAB, // he (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0011111111100ULL),
        L(0x0000001110000ULL),
        L(0x0001111110000ULL),
        L(0x0111000110000ULL),
        L(0x1100000011111ULL), //
        L(0x1100000000111ULL), //
        L(0x0110000000000ULL),
        L(0x0011000000000ULL),
        L(0x0001110000000ULL),
        L(0x0000011111000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aKhe=
{
    0xAC, // khe (allone) 
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000111000ULL),
        L(0x00000111000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00111111111ULL),
        L(0x00000011100ULL),
        L(0x00011110000ULL),
        L(0x01110000000ULL),
        L(0x11000000000ULL), //
        L(0x11000000000ULL), //
        L(0x01100000000ULL),
        L(0x00110000000ULL),
        L(0x00011110000ULL),
        L(0x00000111110ULL),
    },
};

const CharDefinition c_iKhe=
{
    0xAD, // khe (initial and middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00001100000000ULL),
        L(0x00001100000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00011111110000ULL),
        L(0x00000001111000ULL),
        L(0x00000000011100ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eKhe=
{
    0xAE, // khe (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000011000000ULL),
        L(0x0000011000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0011111111100ULL),
        L(0x0000001110000ULL),
        L(0x0001111110000ULL),
        L(0x0111000110000ULL),
        L(0x1100000011111ULL), //
        L(0x1100000000111ULL), //
        L(0x0110000000000ULL),
        L(0x0110000000000ULL),
        L(0x0011110000000ULL),
        L(0x0000011111000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aDal=
{
    0xAF, // dal (all cases)
    9,    // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000110ULL),
        L(0x000000111ULL),
        L(0x000000011ULL),
        L(0x011111111ULL), //
        L(0x000111111ULL), //
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aZal=
{
    0xB0, // zal (all cases)
    9,    // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000011000ULL),
        L(0x000011000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000110ULL),
        L(0x000000111ULL),
        L(0x000000011ULL),
        L(0x011111111ULL), //
        L(0x000111111ULL), //
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aRe=
{
    0xB1, // re (all cases)
    12,  // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000011100ULL),
        L(0x000000011110ULL),
        L(0x000000000110ULL),
        L(0x000000000011ULL), //
        L(0x000000000011ULL), //
        L(0x000000000110ULL),
        L(0x110000011100ULL),
        L(0x011111110000ULL),
        L(0x000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aZe=
{
    0xB2, // ze (all cases)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000011000000ULL),
        L(0x000011000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000011100ULL),
        L(0x000000011110ULL),
        L(0x000000000110ULL),
        L(0x000000000011ULL), //
        L(0x000000000011ULL), //
        L(0x000000000110ULL),
        L(0x110000011100ULL),
        L(0x011111110000ULL),
        L(0x000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aZhe=
{
    0xB3, // zhe (all cases)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000000011ULL),
        L(0x000110000011ULL),
        L(0x000110000000ULL),
        L(0x000000011100ULL),
        L(0x000000011110ULL),
        L(0x000000000110ULL),
        L(0x000000000011ULL), //
        L(0x000000000011ULL), //
        L(0x000000000110ULL),
        L(0x110000011100ULL),
        L(0x011111110000ULL),
        L(0x000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_pSxx=
{
    0xB4, // left part of sin/shin/sad/zad characters
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000011110ULL),
        L(0x010000000111ULL),
        L(0x110000000011ULL),
        L(0x110000000011ULL), //
        L(0x110000000011ULL), //
        L(0x011000000110ULL),
        L(0x011110011100ULL),
        L(0x000111111000ULL),
        L(0x000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aSin=
{
    0xB5, // sin (right part of allone)
    9,    // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000110ULL),
        L(0x001100110ULL),
        L(0x001100111ULL),
        L(0x000110011ULL),
        L(0x111111111ULL), //
        L(0x111111110ULL), //
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};


const CharDefinition c_iSin=
{
    0xB6, // sin (initial)
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0110000000110ULL),
        L(0x0110001100110ULL),
        L(0x0110001100111ULL),
        L(0x0011000110011ULL),
        L(0x1111111111111ULL), //
        L(0x1100111100110ULL), //
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};


const CharDefinition c_mSin=
{
    0xB7, // sin (middle)
    15,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x001100000011000ULL),
        L(0x001100110011000ULL),
        L(0x001100110011100ULL),
        L(0x001100011001100ULL),
        L(0x111111111111111ULL), //
        L(0x111001110011111ULL), //
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
    },
};

const CharDefinition c_eSin=
{
    0xB8, // sin (right part of end)
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000110000ULL),
        L(0x0011000110000ULL),
        L(0x0011000111000ULL),
        L(0x0001100011000ULL),
        L(0x1111111111111ULL), //
        L(0x1111001111111ULL), //
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aShin=
{
    0xB9, // shin (right part of allone)
    10,    // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000110000ULL),
        L(0x0000110000ULL),
        L(0x0011001100ULL),
        L(0x0011001100ULL),
        L(0x0000000000ULL),
        L(0x0011000110ULL),
        L(0x0011000111ULL),
        L(0x0001100011ULL),
        L(0x1111111111ULL), //
        L(0x1111111110ULL), //
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
        L(0x0000000000ULL),
    },
};


const CharDefinition c_iShin=
{
    0xBA, // shin (initial)
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000001100000ULL),
        L(0x0000001100000ULL),
        L(0x0000110011000ULL),
        L(0x0000110011000ULL),
        L(0x0110000000110ULL),
        L(0x0110001100110ULL),
        L(0x0110001100111ULL),
        L(0x0011000110011ULL),
        L(0x1111111111111ULL), //
        L(0x1100111100110ULL), //
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};


const CharDefinition c_mShin=
{
    0xBB, // shin (middle)
    15,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000110000000ULL),
        L(0x000000110000000ULL),
        L(0x000011001100000ULL),
        L(0x000011001100000ULL),
        L(0x001100000011000ULL),
        L(0x001100110011000ULL),
        L(0x001100110011100ULL),
        L(0x001100011001100ULL),
        L(0x111111111111111ULL), //
        L(0x111001110011111ULL), //
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
    },
};

const CharDefinition c_eShin=
{
    0xBC, // shin (right part of end)
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000011000000ULL),
        L(0x0000011000000ULL),
        L(0x0001100110000ULL),
        L(0x0001100110000ULL),
        L(0x0000000000000ULL),
        L(0x0011000110000ULL),
        L(0x0011000111000ULL),
        L(0x0001100011000ULL),
        L(0x1111111111111ULL), //
        L(0x1111001111111ULL), //
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aSad=
{
    0xBD, // sad (right part of allone)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000001111100ULL),
        L(0x000011000110ULL),
        L(0x000110000011ULL),
        L(0x001100000011ULL),
        L(0x111111111111ULL), //
        L(0x111111111111ULL), //
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_iSad=
{
    0xBE, // sad (initial)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000001111110ULL),
        L(0x0001100111000111ULL),
        L(0x0001101100000011ULL),
        L(0x0001111000000011ULL),
        L(0x1111111111111111ULL), //
        L(0x1111001111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_mSad=
{
    0xBF, // sad (middle)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000011111000ULL),
        L(0x0011001110011100ULL),
        L(0x0011011100001100ULL),
        L(0x0011110000001100ULL),
        L(0x1111111111111111ULL), //
        L(0x1110011111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_eSad=
{
    0xC0, // sad (right part of end)
    15,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000011111000ULL),
        L(0x000001110011100ULL),
        L(0x000111000001100ULL),
        L(0x001100000001100ULL),
        L(0x111111111111111ULL), //
        L(0x111111111111111ULL), //
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aZad=
{
    0xC1, // zad (right part of allone)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000110000000ULL),
        L(0x000110000000ULL),
        L(0x000000000000ULL),
        L(0x000001111100ULL),
        L(0x000011000110ULL),
        L(0x000110000011ULL),
        L(0x001100000011ULL),
        L(0x111111111111ULL), //
        L(0x111111111111ULL), //
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_iZad=
{
    0xC2, // zad (initial)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000001100000000ULL),
        L(0x0000001100000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000001111110ULL),
        L(0x0001100111000111ULL),
        L(0x0001101100000011ULL),
        L(0x0001111000000011ULL),
        L(0x1111111111111111ULL), //
        L(0x1111001111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_mZad=
{
    0xC3, // zad (middle)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000011000000000ULL),
        L(0x0000011000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000011111000ULL),
        L(0x0011001110011100ULL),
        L(0x0011011100001100ULL),
        L(0x0011110000001100ULL),
        L(0x1111111111111111ULL), //
        L(0x1110011111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_eZad=
{
    0xC4, // zad (right part of end)
    15,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000000ULL),
        L(0x001100000000000ULL),
        L(0x001100000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000011111000ULL),
        L(0x000001110011100ULL),
        L(0x000111000001100ULL),
        L(0x001100000001100ULL),
        L(0x111111111111111ULL), //
        L(0x111111111111111ULL), //
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aTa=
{
    0xC5, // ta (allone and initial)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00001100000000ULL),
        L(0x00011100000000ULL),
        L(0x00001100000000ULL),
        L(0x00001100000000ULL),
        L(0x00001100111110ULL),
        L(0x00001111100111ULL),
        L(0x00001110000011ULL),
        L(0x00011100000011ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_mTa=
{
    0xC6, // ta (middle and end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000110000000000ULL),
        L(0x0001110000000000ULL),
        L(0x0000110000000000ULL),
        L(0x0000110000000000ULL),
        L(0x0000110011111000ULL),
        L(0x0000111110011100ULL),
        L(0x0000111000001100ULL),
        L(0x0001110000001100ULL),
        L(0x1111111111111111ULL), //
        L(0x1111111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aZa=
{
    0xC7, // za (allone and initial)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00001100000000ULL),
        L(0x00011100001100ULL),
        L(0x00001100001100ULL),
        L(0x00001100000000ULL),
        L(0x00001100111110ULL),
        L(0x00001111100111ULL),
        L(0x00001110000011ULL),
        L(0x00011100000011ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_mZa=
{
    0xC8, // za (middle and end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000110000000000ULL),
        L(0x0001110000110000ULL),
        L(0x0000110000110000ULL),
        L(0x0000110000000000ULL),
        L(0x0000110011111000ULL),
        L(0x0000111110011100ULL),
        L(0x0000111000001100ULL),
        L(0x0001110000001100ULL),
        L(0x1111111111111111ULL), //
        L(0x1111111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aEyn=
{
    0xC9, // eyn (allone)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x001111111000ULL),
        L(0x011000000000ULL),
        L(0x111000000000ULL),
        L(0x011110011110ULL),
        L(0x000011110000ULL),
        L(0x011100000000ULL), //
        L(0x011000000000ULL), //
        L(0x110000000000ULL),
        L(0x111000000000ULL),
        L(0x001111111111ULL),
        L(0x000011111100ULL),
    },
};

const CharDefinition c_iEyn=
{
    0xCA, // eyn (initial)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011111100ULL),
        L(0x001110000000ULL),
        L(0x001110000000ULL),
        L(0x001100000011ULL),
        L(0x111111111111ULL), //
        L(0x111111111000ULL), //
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_mEyn=
{
    0xCB, // eyn (middle)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000001110000ULL),
        L(0x000111111000ULL),
        L(0x001100111100ULL),
        L(0x000000111000ULL),
        L(0x111111111111ULL), //
        L(0x111111001111ULL), //
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_eEyn=
{
    0xCC, // eyn (end)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011110000ULL),
        L(0x001111111100ULL),
        L(0x001001111000ULL),
        L(0x000011110000ULL),
        L(0x001110011111ULL), //
        L(0x011000000111ULL), //
        L(0x110000000000ULL),
        L(0x111000000000ULL),
        L(0x001111111111ULL),
        L(0x000011111100ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aQeyn=
{
    0xCD, // qeyn (allone)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000001100000ULL),
        L(0x000001100000ULL),
        L(0x000000000000ULL),
        L(0x001111111000ULL),
        L(0x011000000000ULL),
        L(0x111000000000ULL),
        L(0x011110011110ULL),
        L(0x000011110000ULL),
        L(0x011100000000ULL), //
        L(0x011000000000ULL), //
        L(0x110000000000ULL),
        L(0x111000000000ULL),
        L(0x001111111111ULL),
        L(0x000011111100ULL),
    },
};

const CharDefinition c_iQeyn=
{
    0xCE, // qeyn (initial)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011111100ULL),
        L(0x001110000000ULL),
        L(0x001110000000ULL),
        L(0x001100000011ULL),
        L(0x111111111111ULL), //
        L(0x111111111000ULL), //
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_mQeyn=
{
    0xCF, // qeyn (middle)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000110000ULL),
        L(0x000000110000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000001110000ULL),
        L(0x000111111000ULL),
        L(0x001100111100ULL),
        L(0x000000111000ULL),
        L(0x111111111111ULL), //
        L(0x111111001111ULL), //
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
    },
};

const CharDefinition c_eQeyn=
{
    0xD0, // qeyn (end)
    12,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000001100000ULL),
        L(0x000001100000ULL),
        L(0x000000000000ULL),
        L(0x000000000000ULL),
        L(0x000011110000ULL),
        L(0x001111111100ULL),
        L(0x001001111000ULL),
        L(0x000011110000ULL),
        L(0x001110011111ULL), //
        L(0x011000000111ULL), //
        L(0x110000000000ULL),
        L(0x111000000000ULL),
        L(0x001111111111ULL),
        L(0x000011111100ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aFe=
{
    0xD1, // fe (allone) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000011000ULL),
        L(0x0000000000011000ULL),
        L(0x0000000000000000ULL),
        L(0x0100000001111110ULL),
        L(0x1100000111000011ULL),
        L(0x1100000111000011ULL),
        L(0x1110000001111111ULL),
        L(0x0111000000000111ULL),
        L(0x0011111111111111ULL), //
        L(0x0000111111111110ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_iFe=
{
    0xD2, // fe (initial)
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000011000ULL),
        L(0x0000000011000ULL),
        L(0x0000000000000ULL),
        L(0x0000001111110ULL),
        L(0x0000111000011ULL),
        L(0x0000111000011ULL),
        L(0x0000001111111ULL),
        L(0x0000000000111ULL),
        L(0x1111111111111ULL), //
        L(0x1111111111110ULL), //
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};

const CharDefinition c_mFe=
{
    0xD3, // fe (middle)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000011000000ULL),
        L(0x0000000011000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000001111110000ULL),
        L(0x0000111000011000ULL),
        L(0x0000111000011000ULL),
        L(0x0000001111111000ULL),
        L(0x0000000000111000ULL),
        L(0x1111111111111111ULL), //
        L(0x1111111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_eFe=
{
    0xD4, // fe (end) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000001100000ULL),
        L(0x0000000001100000ULL),
        L(0x0000000000000000ULL),
        L(0x0100001111111000ULL),
        L(0x1100011100001100ULL),
        L(0x1100011100001100ULL),
        L(0x1110000111111100ULL),
        L(0x0111000000011100ULL),
        L(0x0011111111111111ULL), //
        L(0x0000111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aQaf=
{
    0xD5, // qaf (allone)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000011001100ULL),
        L(0x0000000011001100ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000011111110ULL),
        L(0x0100000110000111ULL),
        L(0x1100000110000011ULL),
        L(0x1100000011111111ULL), //
        L(0x1100000000111111ULL), //
        L(0x0110000000000110ULL),
        L(0x0111100000011100ULL),
        L(0x0001111111110000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_iQaf=
{
    0xD6, // qaf (initial)
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000110001100ULL),
        L(0x0000110001100ULL),
        L(0x0000000000000ULL),
        L(0x0000001111110ULL),
        L(0x0000111000011ULL),
        L(0x0000111000011ULL),
        L(0x0000001111111ULL),
        L(0x0000000000111ULL),
        L(0x1111111111111ULL), //
        L(0x1111111111110ULL), //
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};

const CharDefinition c_mQaf=
{
    0xD7, // qaf (middle)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000011000110000ULL),
        L(0x0000011000110000ULL),
        L(0x0000000000000000ULL),
        L(0x0000001111110000ULL),
        L(0x0000111000011000ULL),
        L(0x0000111000011000ULL),
        L(0x0000001111111000ULL),
        L(0x0000000000111000ULL),
        L(0x1111111111111111ULL), //
        L(0x1111111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_eQaf=
{
    0xD8, // qaf (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000011000110000ULL),
        L(0x0000011000110000ULL),
        L(0x0000000000000000ULL),
        L(0x0000001111111000ULL),
        L(0x0000011000011100ULL),
        L(0x0100011000001100ULL),
        L(0x1100011111111100ULL),
        L(0x1100000111111111ULL), //
        L(0x1100000000001111ULL), //
        L(0x0110000000011000ULL),
        L(0x0111100001110000ULL),
        L(0x0001111111000000ULL),
        L(0x0000000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aKaf=
{
    0xD9, // kaf (allone)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000011ULL),
        L(0x0000000000001110ULL),
        L(0x0000000000111000ULL),
        L(0x0000000111000000ULL),
        L(0x0000001110000000ULL),
        L(0x0000000011100000ULL),
        L(0x1000000001111000ULL),
        L(0x1100000000001110ULL),
        L(0x1111111111111111ULL), //
        L(0x0011111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_iKaf=
{
    0xDA, // kaf (initial)
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000011ULL),
        L(0x00000001110ULL),
        L(0x00000111000ULL),
        L(0x00011100000ULL),
        L(0x00111000000ULL),
        L(0x00011100000ULL),
        L(0x00001111000ULL),
        L(0x00000001110ULL),
        L(0x11111111111ULL), //
        L(0x11111111111ULL), //
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_mKaf=
{
    0xDB, // kaf (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000111ULL),
        L(0x00000000011100ULL),
        L(0x00000011100000ULL),
        L(0x00011100000000ULL),
        L(0x00111000000000ULL),
        L(0x00011100000000ULL),
        L(0x00001111000000ULL),
        L(0x00000011110000ULL),
        L(0x11111110011111ULL), //
        L(0x11111000000111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eKaf=
{
    0xDC, // kaf (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000011100ULL),
        L(0x0000000001110000ULL),
        L(0x0000001110000000ULL),
        L(0x0001110000000000ULL),
        L(0x0011100000000000ULL),
        L(0x0001110000000000ULL),
        L(0x1100001111000000ULL),
        L(0x1100000011111000ULL),
        L(0x1111111110001111ULL), //
        L(0x0011111000000011ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aGaf=
{
    0xDD, // gaf (allone)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000110ULL),
        L(0x0000000000011000ULL),
        L(0x0000000001100001ULL),
        L(0x0000000110000111ULL),
        L(0x0000001000111100ULL),
        L(0x0000000011100000ULL),
        L(0x1000000001111000ULL),
        L(0x1100000000001110ULL),
        L(0x1111111111111111ULL), //
        L(0x0011111111111111ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_iGaf=
{
    0xDE, // gaf (initial)
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000001100ULL),
        L(0x00000011000ULL),
        L(0x00001100011ULL),
        L(0x00110001110ULL),
        L(0x01000111000ULL),
        L(0x00011100000ULL),
        L(0x00001111000ULL),
        L(0x00000001110ULL),
        L(0x11111111111ULL), //
        L(0x11111111111ULL), //
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_mGaf=
{
    0xDF, // gaf (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000110000ULL),
        L(0x00000011000000ULL),
        L(0x00001100011000ULL),
        L(0x00110001110000ULL),
        L(0x01000111000000ULL),
        L(0x00011100000000ULL),
        L(0x00001111000000ULL),
        L(0x00000011110000ULL),
        L(0x11111110011111ULL), //
        L(0x11111000000111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eGaf=
{
    0xE0, // gaf (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000110000ULL),
        L(0x0000000011000000ULL),
        L(0x0000001100011000ULL),
        L(0x0000110001110000ULL),
        L(0x0001000111000000ULL),
        L(0x0000011100000000ULL),
        L(0x1100001111000000ULL),
        L(0x1100000011111000ULL),
        L(0x1111111110001111ULL), //
        L(0x0011111000000011ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aLam=
{
    0xE1, // lam (allone)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000110ULL),
        L(0x00000000001110ULL),
        L(0x00000000001111ULL),
        L(0x00000000000110ULL),
        L(0x00000000000110ULL),
        L(0x00000000000110ULL),
        L(0x01000000000111ULL),
        L(0x11000000000011ULL),
        L(0x11000000000011ULL), //
        L(0x11000000000111ULL), //
        L(0x01110000011110ULL),
        L(0x00011111110000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_iLam=
{
    0xE2, // lam (initial)
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000110ULL),
        L(0x00000001110ULL),
        L(0x00000000111ULL),
        L(0x00000000110ULL),
        L(0x00000000110ULL),
        L(0x00000000110ULL),
        L(0x00000000111ULL),
        L(0x00000000011ULL),
        L(0x11111111111ULL), //
        L(0x11111111111ULL), //
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_mLam=
{
    0xE3, // lam (middle)
    15,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000001100000ULL),
        L(0x000000011100000ULL),
        L(0x000000011100000ULL),
        L(0x000000001100000ULL),
        L(0x000000001100000ULL),
        L(0x000000001100000ULL),
        L(0x000000001110000ULL),
        L(0x000000000110000ULL),
        L(0x111111111111111ULL), //
        L(0x111111111111111ULL), //
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
    },
};

const CharDefinition c_eLam=
{
    0xE4, // lam (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000110000ULL),
        L(0x0000000001110000ULL),
        L(0x0000000001110000ULL),
        L(0x0000000000110000ULL),
        L(0x0000000000110000ULL),
        L(0x0000000000110000ULL),
        L(0x0100000000111000ULL),
        L(0x1100000000011000ULL),
        L(0x1100000000011111ULL), //
        L(0x1100000000111111ULL), //
        L(0x0111000001110000ULL),
        L(0x0001111111000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aMim=
{
    0xE5, // mim (allone)
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00011111100ULL),
        L(0x00110000110ULL),
        L(0x00000000011ULL),
        L(0x00011111111ULL), //
        L(0x11111111111ULL), //
        L(0x11100000000ULL),
        L(0x00111000000ULL),
        L(0x00011000000ULL),
        L(0x00011000000ULL),
    },
};

const CharDefinition c_iMim=
{
    0xE6, // mim (initial)
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000001100ULL),
        L(0x00000111110ULL),
        L(0x00001100111ULL),
        L(0x00011100011ULL),
        L(0x11111111111ULL), //
        L(0x11100011111ULL), //
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_mMim=
{
    0xE7, // mim (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000001100000ULL),
        L(0x00000111110000ULL),
        L(0x00001100111000ULL),
        L(0x00011100011000ULL),
        L(0x11111111111111ULL), //
        L(0x11100011111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eMim=
{
    0xE8, // mim (end)
    15,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000001111000000ULL),
        L(0x000111111100000ULL),
        L(0x001100001110000ULL),
        L(0x000000000110000ULL),
        L(0x000011111111111ULL), //
        L(0x011111111111111ULL), //
        L(0x111000000000000ULL),
        L(0x001110000000000ULL),
        L(0x000110000000000ULL),
        L(0x000110000000000ULL),
    },
};

//--------------------------------------------------
const CharDefinition c_aNun=
{
    0xE9, // nun (allone)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000110000000ULL),
        L(0x00000110000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000001110ULL),
        L(0x01000000000111ULL),
        L(0x11000000000011ULL),
        L(0x11000000000011ULL), //
        L(0x11000000000011ULL), //
        L(0x01100000000110ULL),
        L(0x01110000011100ULL),
        L(0x00011111110000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_iNun=
{
    0xEA, // nun (initial)
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000011000ULL),
        L(0x00000011000ULL),
        L(0x00000000000ULL),
        L(0x00000000110ULL),
        L(0x00000000110ULL),
        L(0x00000000111ULL),
        L(0x00000000011ULL),
        L(0x11111111111ULL), //
        L(0x11111111111ULL), //
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_mNun=
{
    0xEB, // nun (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000011000000ULL),
        L(0x00000011000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000110000ULL),
        L(0x00000000110000ULL),
        L(0x00000000111000ULL),
        L(0x00000000011000ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eNun=
{
    0xEC, // nun (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000011000000000ULL),
        L(0x0000011000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000001100000ULL),
        L(0x0000000001110000ULL),
        L(0x1100000000111000ULL),
        L(0x1100000000011000ULL),
        L(0x1100000000011111ULL), //
        L(0x1100000000011111ULL), //
        L(0x0110000000110000ULL),
        L(0x0111000011100000ULL),
        L(0x0001111110000000ULL),
        L(0x0000000000000000ULL),
    },
};

//--------------------------------------------
const CharDefinition c_aVav=
{
    0xED, // vav (all cases)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000001111100ULL),
        L(0x00000011001110ULL),
        L(0x00000110000111ULL),
        L(0x00000110000011ULL),
        L(0x00000111000011ULL),
        L(0x00000011111111ULL), //
        L(0x00000000011111ULL), //
        L(0x00000000000011ULL),
        L(0x00000000001110ULL),
        L(0x01110000111000ULL),
        L(0x00011111100000ULL),
    },
};


//--------------------------------------------
const CharDefinition c_aHe2=
{
    0xEE, // he2 (allone)
    9,    // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x001110000ULL),
        L(0x011111000ULL),
        L(0x111001100ULL),
        L(0x110000110ULL),
        L(0x111000011ULL),
        L(0x011111111ULL), //
        L(0x001111100ULL), //
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
        L(0x000000000ULL),
    },
};

const CharDefinition c_iHe2=
{
    0xEF, // he (initial)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000111100000000ULL),
        L(0x0000001111100000ULL),
        L(0x0000011001111100ULL),
        L(0x0000110001101110ULL),
        L(0x0000110111000111ULL),
        L(0x1111111111101110ULL), //
        L(0x1111110001111000ULL), //
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_mHe2=
{
    0xF0, // he (middle)
    15,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000000000000000ULL),
        L(0x000011111100000ULL),
        L(0x000110000110000ULL),
        L(0x000110000110000ULL),
        L(0x000110001100000ULL),
        L(0x111111111111111ULL), //
        L(0x111111111111111ULL), //
        L(0x000110000110000ULL),
        L(0x000110000011000ULL),
        L(0x000011100011100ULL),
        L(0x000001111111000ULL),
    },
};

const CharDefinition c_eHe2=
{
    0xF1, // he (end)
    13,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000011000000ULL),
        L(0x0011111100000ULL),
        L(0x0111001100000ULL),
        L(0x1110001110000ULL),
        L(0x1111111111000ULL),
        L(0x0111110011111ULL), //
        L(0x0000000001111ULL), //
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
        L(0x0000000000000ULL),
    },
};

//--------------------------------------------------
const CharDefinition c_aYe=
{
    0xF2, // ye (allone)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000111110ULL),
        L(0x00000011100011ULL),
        L(0x01000111000000ULL),
        L(0x11000111000000ULL),
        L(0x11000111111111ULL), //
        L(0x11000001111111ULL), //
        L(0x01100000000110ULL),
        L(0x01110000011100ULL),
        L(0x00011111110000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_iYe=
{
    0xF3, // ye (initial)
    11,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000000ULL),
        L(0x00000000110ULL),
        L(0x00000000110ULL),
        L(0x00000000111ULL),
        L(0x00000000011ULL),
        L(0x11111111111ULL), //
        L(0x11111111111ULL), //
        L(0x00000000000ULL),
        L(0x00011001100ULL),
        L(0x00011001100ULL),
        L(0x00000000000ULL),
    },
};

const CharDefinition c_mYe=
{
    0xF4, // ye (middle)
    14,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000000000ULL),
        L(0x00000000110000ULL),
        L(0x00000000110000ULL),
        L(0x00000000111000ULL),
        L(0x00000000011000ULL),
        L(0x11111111111111ULL), //
        L(0x11111111111111ULL), //
        L(0x00000000000000ULL),
        L(0x00011001100000ULL),
        L(0x00011001100000ULL),
        L(0x00000000000000ULL),
    },
};

const CharDefinition c_eYe=
{
    0xF5, // ye (end)
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000001111111000ULL),
        L(0x0100011000001100ULL),
        L(0x1100001111000111ULL), //
        L(0x1100000001100011ULL), //
        L(0x0110000000110000ULL),
        L(0x0111111111100000ULL),
        L(0x0001100011000000ULL),
        L(0x0001100011000000ULL),
    },
};

const CharDefinition c_eE1=
{
    0xF6, // 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000011111100000ULL),
        L(0x0001111111110000ULL),
        L(0x0001110000000000ULL),
        L(0x0001110000001110ULL),
        L(0x0000011111111000ULL),
        L(0x0000011111000000ULL),
        L(0x0011110000000000ULL),
        L(0x1111000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};


const CharDefinition c_eE2=
{
    0xF7, // 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000001111000000ULL),
        L(0x0000010000100000ULL),
        L(0x0000001000000000ULL),
        L(0x0000111111110000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000011000ULL),
        L(0x0000000000011000ULL),
        L(0x1111111111111111ULL),
        L(0x1111111111111111ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};


const CharDefinition c_eE3=
{
    0xF8, // 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000011100000000ULL),
        L(0x0000011110000000ULL),
        L(0x0000100000000000ULL),
        L(0x0000001100000000ULL),
        L(0x0000001100000000ULL),
        L(0x0000001100000000ULL),
        L(0x0000001100000000ULL),
        L(0x0000001100000000ULL),
        L(0x0000000111111111ULL),
        L(0x0000000011111111ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};


const CharDefinition c_eE4=
{
    0xF9, // 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000111111100000ULL),
        L(0x0001110000111000ULL),
        L(0x0011100000011000ULL),
        L(0x0111000000000000ULL),
        L(0x0111000000000000ULL),
        L(0x0111000000000000ULL),
        L(0x0001111000000000ULL),
        L(0x0000001110000000ULL),
        L(0x0000000111000000ULL),
        L(0x0000000111000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000111000000ULL),
        L(0x0000000111000000ULL),
    },
};

const CharDefinition c_eE5=
{
    0xFA, // 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000110000000ULL),
        L(0x0000011100000000ULL),
        L(0x0000111000000000ULL),
        L(0x0000111000000000ULL),
        L(0x0000011111000000ULL),
        L(0x0000001111000000ULL),
        L(0x0000001111000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};

const CharDefinition c_eE6=
{
    0xFB, // 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000001111000000ULL),
        L(0x0000010000100000ULL),
        L(0x0000001000000000ULL),
        L(0x0000111111110000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000011000ULL),
        L(0x0000000000011000ULL),
        L(0x1111111111111000ULL),
        L(0x1111111111110000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};


const CharDefinition c_UzatmaCizgisi=
{
    0xFD, // this is default (all unknows will be printed as default) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x1111111111111111ULL),
        L(0x1111111111111111ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};
const CharDefinition c_Empty=
{
    0xFF, // this is default (all unknows will be printed as default) 
    16,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
        L(0x0000000000000000ULL),
    },
};
const CharDefinition c_OnePixelEmpty=
{
    0xFE, // this is default (all unknows will be printed as default) 
    1,   // Character width in pixels
    0x00, // Character options bitmap
    {
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
        L(0x0ULL),
    },
};

#endif // MAKE_FARSI

