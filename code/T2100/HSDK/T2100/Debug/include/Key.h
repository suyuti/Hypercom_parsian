
///////////////////////////////////////////////////////////////////////////
//
// Module   : KEY
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module KEY
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
// 030602  BW      Original version (based on EFTOS8 version 020712)
// 050112  RZ      Removed K_Test & K_Scan functions
//
///////////////////////////////////////////////////////////////////////////

#ifndef KEY_H
#define KEY_H

extern BOOL volatile bK_Evt;

extern LPBYTE pK_Tab;

// Keyboard functions
void K_BufClear(void);
void K_BufIn(BYTE nNewKey);
void K_Open(void);
BYTE K_PressedKey(void);
BOOL K_Test(void);


#endif
