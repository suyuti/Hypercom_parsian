///////////////////////////////////////////////////////////////////////////
//
// Module   : BCIHW
// Part of  : SSW, EFTOS
// Created  : Roman Zaytsev
// Abstract : Header file of module BCIHW
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
// 041012  RZ      Original version
//
///////////////////////////////////////////////////////////////////////////

#ifndef BCIHW_H
#define BCIHW_H

#include "SswDef.h"

// Define Rx/Tx bits
#define BCI_RX_BIT	4
#define BCI_TX_BIT	6


// Function declarations
void S_BciInit(void);
BOOL S_BciCheck(void);
void S_BciClose(void);
BOOL S_BciRx(void);
void S_BciTx(BOOL bState);

#endif

