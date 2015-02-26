
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
// 030506  BW      Original version (based on EFTOS8 version 020712)
// 030603  BW      Removed function D_EnBufUpdate
// 030625  BW      Added new functions to define font table to be used
// 030630  BW      Added definitions and functions to handle font sizes
// 031104  JB      Added new function D_AdjContrast for adjusting contrast and
//				   D_RestContrast, restores contrast to default value.
// 040415  JB      New Function D_BackLight, to turn backlight on and off
// 040930  RZ      Added BackLight scan function
// 041103  RZ      Added Screen Grabber function
// 050112  RZ      Removed useless staff
//
///////////////////////////////////////////////////////////////////////////

#ifndef DSP_H
#define DSP_H

// Display size
// Number of rows (each row is 8 pixel high)
#define D_NO_ROW          8

// Number of pixel in X direction
#define D_NO_PIXEL_X      128

// Number of pixel in Y direction (actually 65 but only 64 are used)
#define D_NO_PIXEL_Y      64

// Defines of direction when drawing
#define D_DIR_HOR  0    // Horizontal
#define D_DIR_VER  1    // Vertical

// Defines of mode when drawing
#define D_MODE_NORMAL               0
#define D_MODE_XOR                  1
#define D_MODE_NORMAL_CLEAR_BKGND   2


// Define possible font sizes
#define D_FONT_SIZE_NORMAL          0
#define D_FONT_SIZE_LARGE           1

// Available fonts
#define D_FONT_MAX					8


// Display functions
void D_Clear(void);
void D_ClearPixel(DWORD nPixelPosX, DWORD nPixelPosY);
void D_CurOff(void);
void D_CurOn(void);
void D_UpdOff(void);
void D_UpdOn(void);
void D_DefCurPos(DWORD nNewCharRow, DWORD nNewCharCol);
DWORD D_DefFont(const BYTE* pFont, DWORD nSizeCharX, DWORD nSizeCharY);
void D_DefFontSize(DWORD nNewFontSize);
void D_Disp(LPCSTR pcszText, DWORD nLen);
void D_DispStr(LPCSTR pcszText);
void D_DrawLine(DWORD nPixelPosX, DWORD nPixelPosY,
                DWORD Direction, DWORD nLength, DWORD Mode);
void D_DrawPic(DWORD nPixelPosX, DWORD nPixelPosY,
               DWORD nSizeX, DWORD nSizeY, LPBYTE pPicture);
void D_Emit(BYTE nCharToWrite);
void D_RetCurPos(DWORD* pCharRow, DWORD* pCharCol);
DWORD D_RetFontSize(void);
void D_ReverseChar(DWORD nCharRow, DWORD nCharCol, DWORD nCharLength);
void D_ReverseCharRow(DWORD nCharRow);
void D_ReverseRow(DWORD nDispRow);
void D_SetPixel(DWORD nPixelPosX, DWORD nPixelPosY);
void D_AdjContrast(BOOL bContUpDown);
void D_RestContrast(void);
void D_BackLight(BOOL bOnOff);


// Functions used internally by the system software
typedef BYTE D_ROW[D_NO_PIXEL_X];
D_ROW * D_GetBuf(void);
void D_Open(void);

#endif
