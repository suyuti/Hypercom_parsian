
///////////////////////////////////////////////////////////////////////////
//
// Module   : BUZ
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module BUZ
//
//         Copyright (C) 2002 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 020712  BW      Original version (based on EFTOS7 version 980129)
//
///////////////////////////////////////////////////////////////////////////

#ifndef BUZ_H
#define BUZ_H

#define B_TIME_2S           20

// Command States
#define B_STATE_IDLE        0
#define B_STATE_BEEP        1
#define B_STATE_BUZZER_ON   3
#define B_STATE_BUZZER_OFF  4

// Buzzer
void B_100MS(void);
void B_Beep(void);
void B_BuzzOff(void);
void B_BuzzOn(void);
DWORD B_GetBeepTime(void);
DWORD B_GetCmdState(void);
void B_Open(DWORD nTimeBuzzBeep, DWORD nTimeBuzzOn, DWORD nTimeBuzzOff);
void B_SetBeepTime(DWORD nTime);

#endif
