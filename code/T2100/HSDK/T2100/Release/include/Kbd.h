
///////////////////////////////////////////////////////////////////////////
//
// Module   : KBD
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module KBD
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
// 030424  BW      Original version (based on EFTOS8 version 020712)
// 050112  RZ      Removed KBD_IsOP_Conn function.
//
///////////////////////////////////////////////////////////////////////////

#ifndef KBD_H
#define KBD_H

// Keyboard/Buzzer/Display handler functions
void KBD_ChKeyTab(LPBYTE pKeyTab);
void KBD_Exec(void);
void KBD_Init(void);
BOOL KBD_IsRS_OP_Conn(void);
void KBD_Open(void);

#endif
