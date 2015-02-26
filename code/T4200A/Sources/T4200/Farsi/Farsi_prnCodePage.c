#ifdef MAKE_FARSI

//=============================================================================
//                           Altius-It
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             Farsi_prnCodePage.c
//      Support Farsi Code page to convert ASCII code to local character set
//=============================================================================


#include <string.h>
#include <stdlib.h>
#include "basictyp.h"
#include "sdk.h" // OS_RETURN_VALS

#include "Farsi_print.h"
#include "Farsi_prnCodePage.h"
#include "Farsi_prnFontNormal.h"

//=============================================================================
// External variables / declarations
//=============================================================================



//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


Farsi_prnCodePage_record Farsi_prnCodePage_table[256] =
{
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x00,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x01,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x02,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x03,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x04,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x05,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x06,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x07,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP /*| o_FN*/ },
{ 0x08,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x09,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x0A,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,             o_LF },
{ 0x0B,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   BAR_ON,        o_ST },
{ 0x0C,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP | o_FF },
{ 0x0D,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x0E,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP | o_FN },// DOUBLE on T42xx
{ 0x0F,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP | o_ST },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x10,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   UND_OFF,o_NP | o_ST },
{ 0x11,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   UND_ON, o_NP | o_ST },
{ 0x12,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   BLD_OFF,o_NP | o_ST },
{ 0x13,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   BLD_ON, o_NP | o_ST },
{ 0x14,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   ITL_OFF,o_NP | o_ST },
{ 0x15,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   ITL_ON, o_NP | o_ST },
{ 0x16,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x17,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP | o_FN },
{ 0x18,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x19,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x1A,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x1B,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x1C,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x1D,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x1E,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
{ 0x1F,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      o_NP },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x20,  &c_Space,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x21,  &c_Exclam,NULL,  NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x22,  &c_Quotes,NULL,  NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x23,  &c_Grate,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x24,  &c_Dollar,NULL,  NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x25,  &c_Prcnt,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x26,  &c_At,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x27,  &c_Apstr,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x28,  &c_Lbrck,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x29,  &c_Rbrck,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x2A,  &c_Astr, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x2B,  &c_Plus, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x2C,  &c_Comma,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x2D,  &c_Minus,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x2E,  &c_Dot,  NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x2F,  &c_Slsh, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x30,  &c_0,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x31,  &c_1,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x32,  &c_2,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x33,  &c_3,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x34,  &c_4,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x35,  &c_5,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x36,  &c_6,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x37,  &c_7,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x38,  &c_8,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x39,  &c_9,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x3A,  &c_Colon,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x3B,  &c_Smcln,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x3C,  &c_Less, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x3D,  &c_Equal,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x3E,  &c_More, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x3F,  &c_Quest,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x40,  &c_Dog,  NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x41,  &c_A,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x42,  &c_B,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x43,  &c_C,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x44,  &c_D,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x45,  &c_E,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x46,  &c_F,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x47,  &c_G,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x48,  &c_H,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x49,  &c_I,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x4A,  &c_J,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x4B,  &c_K,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x4C,  &c_L,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x4D,  &c_M,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x4E,  &c_N,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x4F,  &c_O,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x50,  &c_P,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x51,  &c_Q,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x52,  &c_R,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x53,  &c_S,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x54,  &c_T,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x55,  &c_U,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x56,  &c_V,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x57,  &c_W,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x58,  &c_X,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x59,  &c_Y,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x5A,  &c_Z,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x5B,  &c_LRBrk,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x5C,  &c_BkSls,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x5D,  &c_RRBrk,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x5E,  &c_Cover,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x5F,  &c_Undln,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x60,  &c_Pfstr,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x61,  &c_a,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x62,  &c_b,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x63,  &c_c,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x64,  &c_d,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x65,  &c_e,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x66,  &c_f,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x67,  &c_g,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x68,  &c_h,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x69,  &c_i,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x6A,  &c_j,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x6B,  &c_k,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x6C,  &c_l,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x6D,  &c_m,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x6E,  &c_n,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x6F,  &c_o,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x70,  &c_p,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x71,  &c_q,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x72,  &c_r,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x73,  &c_s,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x74,  &c_t,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x75,  &c_u,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x76,  &c_v,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x77,  &c_w,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x78,  &c_x,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x79,  &c_y,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x7A,  &c_z,    NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x7B,  &c_LFBrk,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x7C,  &c_Stick,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x7D,  &c_RFBrk,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x7E,  &c_Wave, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x7F,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x80,   &c_fa0, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x81,   &c_fa1, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x82,   &c_fa2, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x83,   &c_fa3, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x84,   &c_fa4, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x85,   &c_fa5, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x86,   &c_fa6, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x87,   &c_fa7, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x88,   &c_fa8, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x89,   &c_fa9, NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x8A,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x8B,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x8C,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x8D,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x8E,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x8F,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0x90,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x91,   &c_aAlef,NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x92,   &c_iAlef,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x93,   &c_mAlef,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x94,   &c_aBe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x95,   &c_iBe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x96,   &c_mBe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x97,   &c_eBe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x98,   &c_aPe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x99,   &c_iPe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x9A,   &c_mPe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x9B,   &c_ePe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x9C,   &c_aTe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x9D,   &c_iTe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x9E,   &c_mTe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0x9F,   &c_eTe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0xA0,   &c_aSe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA1,   &c_iSe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA2,   &c_mSe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA3,   &c_eSe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA4,   &c_aJim,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA5,   &c_iJim,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA6,   &c_eJim,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA7,   &c_aChe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA8,   &c_iChe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xA9,   &c_aHe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xAA,   &c_iHe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xAB,   &c_eHe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xAC,   &c_aKhe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xAD,   &c_iKhe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xAE,   &c_eKhe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xAF,   &c_aDal, NULL,  NULL,NULL,  NULL,NULL,  NULL,NULL,   0,      0 },//alef2
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0xB0,   &c_aZal,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB1,   &c_aRe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB2,   &c_aZe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB3,   &c_aZhe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB4,   &c_pSxx,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB5,   &c_aSin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB6,   &c_iSin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB7,   &c_mSin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB8,   &c_eSin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xB9,   &c_aShin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xBA,   &c_iShin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xBB,   &c_mShin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xBC,   &c_eShin,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xBD,   &c_aSad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xBE,   &c_iSad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xBF,   &c_mSad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0xC0,   &c_eSad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC1,   &c_aZad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC2,   &c_iZad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC3,   &c_mZad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC4,   &c_eZad,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC5,   &c_aTa,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC6,   &c_mTa,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC7,   &c_aZa,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC8,   &c_mZa,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xC9,   &c_aEyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xCA,   &c_iEyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xCB,   &c_mEyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xCC,   &c_eEyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xCD,   &c_aQeyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xCE,   &c_iQeyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xCF,   &c_mQeyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0xD0,   &c_eQeyn,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD1,   &c_aFe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD2,   &c_iFe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD3,   &c_mFe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD4,   &c_eFe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD5,   &c_aQaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD6,   &c_iQaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD7,   &c_mQaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD8,   &c_eQaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xD9,   &c_aKaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xDA,   &c_iKaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xDB,   &c_mKaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xDC,   &c_eKaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xDD,   &c_aGaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xDE,   &c_iGaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xDF,   &c_mGaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0xE0,   &c_eGaf,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE1,   &c_aLam,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE2,   &c_iLam,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE3,   &c_mLam,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE4,   &c_eLam,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE5,   &c_aMim,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE6,   &c_iMim,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE7,   &c_mMim,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE8,   &c_eMim,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xE9,   &c_aNun,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xEA,   &c_iNun,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xEB,   &c_mNun,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xEC,   &c_eNun,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xED,   &c_aVav,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xEE,   &c_aHe2,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xEF,   &c_iHe2,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
{ 0xF0,   &c_mHe2,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF1,   &c_eHe2,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF2,   &c_aYe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF3,   &c_iYe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF4,   &c_mYe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF5,   &c_eYe,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF6,   &c_eE1,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//{ 0xF6,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF7,   &c_eE2,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF8,   &c_eE3,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xF9,   &c_eE4,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xFA,   &c_eE5,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xFB,   &c_eE6,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xFC,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xFD,   &c_UzatmaCizgisi,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xFE,   &c_OnePixelEmpty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
{ 0xFF,   &c_Empty,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   NULL,   0,      0 },
//--------------------------------------------------------------------------------------
// ASCII     ALLONE:        INITIAL:         MIDDLE:          END:        COMM-   OPTI-
//        left    rigth   left    rigth   left    rigth   left    rigth   MAND:   ONS: 
//--------------------------------------------------------------------------------------
};



//=============================================================================
// Private data definitions
//=============================================================================

#endif // MAKE_FARSI
