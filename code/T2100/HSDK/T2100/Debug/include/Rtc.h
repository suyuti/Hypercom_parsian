
///////////////////////////////////////////////////////////////////////////
//
// Module   : RTC
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module RTC
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

#ifndef RTC_H
#define RTC_H

// After RTC_INIT, at least the time must be set.

// Parameter buffer for date and time shall hold the following order

// Date buffer (in ASCII)
// Byte 0: HIGH (year)   30H - 39H
// Byte 1: LOW (year)    30H - 39H
// Byte 2: HIGH (month)  30H - 31H
// Byte 3: LOW (month)   30H - 39H
// Byte 4: HIGH (date)   30H - 33H
// Byte 5: LOW (date)    30H - 39H
// Byte 6: Week day      30H - 36H

// Time buffer (in ASCII)
// Byte 0: HIGH (hour)   30H - 32H
// Byte 1: LOW (hour)    30H - 39H
// Byte 2: HIGH (minute) 30H - 35H
// Byte 3: LOW (minute)  30H - 39H
// Byte 4: HIGH (second) 30H - 35H
// Byte 5: LOW (second)  30H - 39H


// Real Time Clock functions
void RTC_GetDate(LPBYTE pBuff);
void RTC_GetTime(LPBYTE pBuff);
DWORD RTC_SetDate(LPBYTE pBuff);
DWORD RTC_SetTime(LPBYTE pBuff);
// Function for internal use
void RTC_Init(DWORD nCentury);

#endif
