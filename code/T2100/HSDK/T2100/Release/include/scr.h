
///////////////////////////////////////////////////////////////////////////
//
// Module   : SCR
// Part of  : SSW, EFTOS
// Created  : Jonas Blick
// Abstract : Header file of module SCR
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
// 040419  JB      Original version
// 050204  RZ      Removed SCR_100MS function 
// 050608  RZ      Added SCR_RetSCOutEvtPtr function.
//
///////////////////////////////////////////////////////////////////////////

#ifndef SCR_H
#define SCR_H


// SCR Public function
void  SCR_Clear(void);
BOOL  SCR_CardIsInserted(void);
BOOL* SCR_RetSCRdyEvtPtr(void);
BOOL* SCR_RetSCOutEvtPtr(void);

//Called by system software
void SCR_Init(void);

#endif
