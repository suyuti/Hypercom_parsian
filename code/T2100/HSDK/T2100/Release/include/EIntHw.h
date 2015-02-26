///////////////////////////////////////////////////////////////////////////
//
// Module   : EINTHW
// Part of  : SSW, EFTOS
// Created  : Roman Zaytsev
// Abstract : Header file of module EINTHW
//
//         Copyright (C) 2004 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 040921  RZ      Original version
//
///////////////////////////////////////////////////////////////////////////

#ifndef EINTHW_H
#define EINTHW_H

#include "SswDef.h"

// Interrupt number
#define EXT_INT_4	0
#define EXT_INT_5	1
#define EXT_INT_6	2
#define EXT_INT_7	3

// Interrupt pending bit
#define EXT_INT_PND_4	0x00000001
#define EXT_INT_PND_5	0x00000002
#define EXT_INT_PND_6	0x00000004
#define EXT_INT_PND_7	0x00000008

// Interrupt signaling method
#define LOW_LEVEL_INT		0x00
#define HIGH_LEVEL_INT		0x01
#define FALLING_EDGE_INT	0x02
#define RISING_EDGE_INT		0x04
#define BOTH_EDGE_INT		0x06
#define ALL_METHOD_MASK		0x07


// Table of pointers to interrupt procedure
extern PROCPTR S_ExtIntPtr[4];

// Function declarations
void S_EnableEINT(DWORD nEInt, DWORD nMode, PROCPTR pFunc);
void S_DisableEINT(DWORD nEInt);

#endif
