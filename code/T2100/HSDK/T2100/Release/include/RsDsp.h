
///////////////////////////////////////////////////////////////////////////
//
// Module   : DSP
// Part of  : SSW, EFTOS
// Created  : Bengt Westman
// Abstract : Header file of module DSP
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
// 030428  BW      Original version (based on EFTOS8 version 020712)
//
///////////////////////////////////////////////////////////////////////////

#ifndef RSDSP_H
#define RSDSP_H

// Display buffer size
#define RSD_BUF_SIZE          250

// Display size
#define RSD_DISPLAY_LENGTH    32
#define RSD_LINE_LENGTH       16

// Display buffer
extern BYTE RSD_Buf[RSD_BUF_SIZE];

// Position of (start of) window in buffer
extern DWORD nRSD_BufPos;

// Number of char to blink
extern DWORD nRSD_BlinkCnt;

// Start position of blinking field
extern DWORD nRSD_BlinkStartPos;

// Status flags
extern BOOL bRSD_StatEnUpdateKBD;
extern BOOL bRSD_StatDspOn;

extern BOOL bRSD_DspStatCurBlinkOn;
extern BOOL bRSD_DspStatBlinkOn;


// Display functions
void RSD_500MS(void);
void RSD_BlinkOff(void);
void RSD_BlinkOn(DWORD nBlinkStartPos, DWORD nBlinkCnt);
void RSD_BufDec(void);
void RSD_BufInc(void);
void RSD_BufInit(void);
void RSD_CurDis(void);
void RSD_CurEn(void);
void RSD_CurOff(void);
void RSD_CurOn(void);
DWORD RSD_CurPos(void);
void RSD_Disp(LPCSTR pcszText, DWORD nLen);
void RSD_DisplayOff(void);
void RSD_DisplayOn(void);
void RSD_Emit(BYTE nCharToWrite);
void RSD_EnBufUpdate(void);
void RSD_Open(void);
void RSD_Put(LPCSTR pcszText);
void RSD_ScrollOff(void);
void RSD_ScrollOn(void);
void RSD_SetCur(DWORD nNewCurPos);
void RSD_UpdOff(void);
void RSD_UpdOn(void);

#endif
