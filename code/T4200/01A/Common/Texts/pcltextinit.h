
#ifndef _PCLTEXTINIT_H_
#define _PCLTEXTINIT_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pcltextinit.h
//      PCL text intialization functions
//
//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
//! Add English PCL messages table for DML support
extern void PclTextEn_Init(void);
//! Add German PCL messages table for DML support
extern void PclTextGr_Init(void);
//! Add English PCL messages table for DML support
extern void PclTextSw_Init(void);
//! Add Spanish PCL messages table for DML support
extern void PclTextEs_Init(void);
//! Add Portuguese PCL messages table for DML support
extern void PclTextPt_Init(void);
//! Add PCL messages table for Dynamic Multilingual Support
extern void PclText_Init(void);

#endif	// _PCLTEXTINIT_H_

